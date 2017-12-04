#ifndef PARQUET_HPP
#define PARQUET_HPP

#include <string>
#include <memory.h>
#include <cassert>
#include <fstream>
#include <iostream>
#include <list>
#include <type_traits>
#include <typeinfo>

#include <boost/variant/variant.hpp>
#include <boost/variant/get.hpp>

#include <arrow/io/file.h>
#include <parquet/file/writer.h>
#include <parquet/file/reader.h>
#include <parquet/api/reader.h>
#include <parquet/api/writer.h>
#include <parquet/schema.h>
#include <parquet/column/scanner.h>
#include <parquet/column/properties.h>
#include <parquet/file/reader-internal.h>
#include <parquet/compression.h>
#include <parquet/util/bit-util.h>
#include <parquet/encoding.h>
#include <parquet/encoding-internal.h>
#include <parquet/util/bit-util.h>

#include "fileinfo.hpp"
#include "Exception/marcoexception.hpp"

namespace SSDK
{
    namespace Archive
    {
        /**
         *  @brief Parquet
         *
         *  @author rime
         *  @version 1.00 2017-05-22 rime
         *                note:create it
         *
         * Parquet简要说明:
         *  1.Parquet支持嵌套的数据模型，类似于Protocol Buffers，每一个数据模型的schema包含多个字段，每一个字段又可以包含多个字段
         *     每一个字段有三个属性：重复数、数据类型和字段名，重复数可以是以下三种：
         *          required(出现1次)，
         *          repeated(出现0次或多次)，
         *          optional(出现0次或1次)。
         *      每一个字段的数据类型可以分成两种：group(复杂类型)和primitive(基本类型)
         * ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
         *
         *  2.Parquet的每一列值都包好了三部分:value、repetition level和definition level。
         *          Value: 具体的值, 详细见Parquet::Type
         *          Repetition Levels:
         *                  值等于它和前面的值在哪一层节点是不共享的。在读取的时候根据该值可以推导出哪一层上需要创建一个新的节点
         *                  例如对于这样的一个schema和两条记录。
         *                            message nested
         *                           {
         *                                   repeated group leve1
         *                                   {
         *                                           repeated string leve2;
         *                                   }
         *                           }
         *
         *                               r1:[[a,b,c,] , [d,e,f,g]]
         *                               r2:[[h] , [i,j]]
         *               计算repetition level值的过程如下：
         *                   value=a是一条记录的开始，和前面的值(已经没有值了)在根节点(第0层)上是不共享的，所以repeated level=0.
         *                   value=b它和前面的值共享了level1这个节点，但是level2这个节点上是不共享的，所以repeated level=2.
         *                   同理value=c, repeated level=2.
         *                   value=d和前面的值共享了根节点(属于相同记录)，但是在level1这个节点上是不共享的，所以repeated level=1.
         *                   value=h和前面的值不属于同一条记录，也就是不共享任何节点，所以repeated level=0.
         *
         *               注意:
         *                      1.可以看出repeated level=0表示一条记录的开始，并且repeated level的值只是针对路径上的repeated类型的节点
         *                      2.计算该值的时候可以忽略非repeated类型的节点，在写入的时候将其理解为该节点和路径上的哪一个repeated节点是不共享的
         *                      3.取的时候将其理解为需要在哪一层创建一个新的repeated节点
         *                      4.每一列最大的repeated level值就等于路径上的repeated节点的个数（不包括根节点）
         *                      5.减小repeated level的好处能够使得在存储使用更加紧凑的编码方式，节省存储空间。
         *
         *      Definition Levels:
         *              值仅仅对于空值是有效的，表示在该值的路径上第几层开始是未定义的，对于非空的值它是没有意义的，因为非空值在叶子节点是定义的，
         *      所有的父节点也肯定是定义的，因此它总是等于该列最大的definition levels
         *                              message ExampleDefinitionLevel
         *                              {
         *                                      optional group a
         *                                      {
         *                                              optional group b
         *                                              {
         *                                                  optional string c;
         *                                              }
         *                                      }
         *                              }
         *
         *                               Value            |           Definition Levels
         *                  --------------------------------------------------------------
         *                               a:null            |                        0
         *                  --------------------------------------------------------------
         *                               b:null            |                        1
         *                   --------------------------------------------------------------
         *                               c:null            |                        2
         *                   --------------------------------------------------------------
         *                               c:"foo"          |                        3
         *                      注意:
         *                              1.由于definition level只需要考虑未定义的值，而对于required类型的节点，只要父节点是已定义的，该节点就必须定义
         *                              2.所以计算definition level的值时可以忽略路径上的required节点，这样可以减小definition level的最大值，优化存储。
         *
         *
         *---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
         *  3.Parquet文件是以二进制方式存储的，所以是不可以直接读取的，文件中包括该文件的数据和元数据
         *      数据:一个RowGroup的vector, 每一个RowGroup包含一定的行数,Parquet读写的时候会将整个行组缓存在内存中，
         *              所以如果每一个行组的大小是由内存大小决定的，例如记录占用空间比较小的Schema可以在每一个行组中存储更多的行。
         *      元数据:包含了"file schema"和RowGroup的列属性
         *              file schema是一个节点树, 每一个节点是简单的类型(叶节点)或者复杂类型(嵌套节点)
         *
         * 具体请参照网址:
         *          <新一代列式存储格式Parquet>    http://blog.csdn.net/yu616568/article/details/50993491
         * 详细请阅读文档:
         *          <<Parquet格式说明.md>>
         *
         */
        class Parquet
        {
        public:

            //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //struct & enum & using

            /**
             *表示Parquet一列的数据结构, 之所以用vector是因为Parquet是按照列存储的, 直接把每列的数据封装在一起
             * 便于与Parquet-cpp进行交互, 减小数据的转化, 提高效率
             *
             * 注意:这里需要特别注意内存释放问题, 具体见文档<<Parquet格式说明.md>>中的 "代码中Parquet的一些注意事项"
             */
            template <class T>
            struct ColumnVector
            {
            public:
                ColumnVector(uint valCnt);
                ColumnVector();

                std::vector<T>& values() {return m_values;}
                std::vector<int16_t>& definitionLevels() {return m_definitionLevels;}
                std::vector<int16_t>& repetitionLevels() {return m_repetitionLevels;}
                std::vector<bool>& isNulls() {return m_isNulls;}
                uint valCnt() const{return m_valCnt;}

                void init(uint valCnt);
                T& getVal(uint valIndex);

                //释放内存
                //注意: 目前只有在一种场合需要调用dispose:
                //       当T的类型为parquet::ByteArray的时候
                //因为Parquet-cpp项目中的ByteArray目前还有缺陷,所以在读取类型为ByteArray中时, 需要手动调用下dispose函数
                void dispose();

            private:
                std::vector<T> m_values;
                std::vector<int16_t> m_definitionLevels;
                std::vector<int16_t> m_repetitionLevels;
                std::vector<bool> m_isNulls;
                uint m_valCnt;
                bool m_isDisposed{false};
            };//End of ColumnVector

            //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

            //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //constructor & deconstructor

            Parquet();
            Parquet(std::vector<parquet::schema::NodePtr>& fields);
            Parquet(const std::string& filePath);
            virtual ~Parquet();

            //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

            //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //get & set functions

            /**
             * @brief rowsPerRowGroup
             * @return 每一个RowGroup的行数
             */
            static uint rowsPerRowGroup() ;
            /**
             * @brief nodePtr
             * @return Schma的Node的头指针
             */
            parquet::schema::NodePtr nodePtr() const{return this->m_nodePtr;}
            /**
             * @brief reader
             * @return 读取Parquet文件的Reader指针
             *
             * 注意:
             *          只有当读取Parquet文件时, 即通过Parquet(const std::string& filePath)或者readFromFilePath(const std::string& filePath)
             * 函数时, 返回不为null, 其余情况都为null
             */
            const std::unique_ptr<parquet::ParquetFileReader>& reader() const{return this->m_reader;}

            //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

            //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //read functions

            /**
             * @brief readFromFilePath
             *             读取一个Parquet文件
             * @param filePath
             *             文件路径
             *
             *   如何路径不存在, 会抛出异常
             */
            void readFromFilePath(const std::string& filePath);

            /**
             * @brief readIndicatedColumn
             *             读取指定的列的所有值
             * @param columnVector
             *              用于存储查询结果的columnVector结构
             * @param columnIndex
             *              指定的列索引, 如果超出了最大列, 会抛出异常
             * @return
             *
             *     注意:
             *               1.该函数由于设计的既可以内部调用, 循环遍历出所有列, 也可以单独外部调用查询指定列
             *               2.调用该函数的时候模板T(来自与内置类型)必须与Parquet-cpp的类型严格对应, 否则将导致编译不通过
             */
            template<typename T>
            typename std::enable_if<std::is_same<T, int>::value ||
                                                    std::is_same<T, int64_t>::value ||
                                                    std::is_same<T, float>::value ||
                                                    std::is_same<T, double>::value ||
                                                    std::is_same<T, bool>::value ||
                                                    std::is_same<T, parquet::ByteArray>::value ||
                                                    std::is_same<T, parquet::Int96>::value>::type
            readIndicatedColumn(ColumnVector<T>& columnVector, uint& columnIndex);

            /**
             * @brief readIndicatedRowValue
             *             读取制定行索引的值
             * @param colReader
             *              列读取器,由于Paruqet是按列驱动的, 所以先读列再读行,一定需要colReader
             * @param columnVector
             *               用于存储查询结果的columnVector结构
             * @param rowIndex
             *                行索引
             */
            template<typename T>
            typename std::enable_if<std::is_same<T, int>::value ||
                                                    std::is_same<T, int64_t>::value ||
                                                    std::is_same<T, float>::value ||
                                                    std::is_same<T, double>::value ||
                                                    std::is_same<T, parquet::Int96>::value>::type
            readIndicatedRowValue(  std::shared_ptr<parquet::ColumnReader>& colReader, ColumnVector<T>& columnVector, uint rowIndex);
            /**
             *和上面的重载函数类型, 只不过收窄了只能处理bool类型
             */
            template<typename T>
            typename std::enable_if<std::is_same<T, bool>::value>::type//bool需要单独处理
            readIndicatedRowValue( std::shared_ptr<parquet::ColumnReader>& colReader, ColumnVector<T>& columnVector, uint rowIndex);
            /**
             *和上面的重载函数类型, 只不过收窄了只能处理ByteArray类型
             */
            template<typename T>
            typename std::enable_if<std::is_same<T, parquet::ByteArray>::value>::type//除了bool以外的所有类型
            readIndicatedRowValue(  std::shared_ptr<parquet::ColumnReader>& colReader, ColumnVector<T>& columnVector, uint rowIndex);

            /**
             * @brief readAllColumns
             *             一次性读取Parquet的所有列数据
             * @param resVals
             *              用于存储查询结果, 这是包含了多个不同类型的columnVector结构
             *
             * 注意:
             *           该函数是可变参数模板函数, resVals包好了多个不同类型的columnVector结构,顺序必须和Parquet文件定义
             *            的列属性一一对应, 否则将导致编译不通过
             */
            template<class... Args>
            void readAllColumns(Args&& ... resVals);

            /**
             *以下所有的getColReaderPtr重载模板函数都是为了获取到正确的ColumnReader指针使用的
             */

            template<typename T>//bool
            typename std::enable_if<std::is_same<T, bool>::value, parquet::BoolReader*>::type
            getColReaderPtr(parquet::ColumnReader* pColReader);

            template<typename T>//Int32
            typename std::enable_if<std::is_same<T, int>::value, parquet::Int32Reader*>::type
            getColReaderPtr(parquet::ColumnReader* pColReader);

            template<typename T>//Int64
            typename std::enable_if<std::is_same<T, int64_t>::value, parquet::Int64Reader*>::type
            getColReaderPtr(parquet::ColumnReader* pColReader);

            template<typename T>//Int96
            typename std::enable_if<std::is_same<T, parquet::Int96>::value, parquet::Int96Reader*>::type
            getColReaderPtr(parquet::ColumnReader* pColReader);

            template<typename T>//float
            typename std::enable_if<std::is_same<T, float>::value, parquet::FloatReader*>::type
            getColReaderPtr(parquet::ColumnReader* pColReader);

            template<typename T>//double
            typename std::enable_if<std::is_same<T, double>::value, parquet::DoubleReader*>::type
            getColReaderPtr(parquet::ColumnReader* pColReader);

            template<typename T>//ByteArray
            typename std::enable_if<std::is_same<T, parquet::ByteArray>::value, parquet::ByteArrayReader*>::type
            getColReaderPtr(parquet::ColumnReader* pColReader);

            template<typename T>//FixedLenByteArray
            typename std::enable_if<std::is_same<T, parquet::FixedLenByteArray>::value, parquet::FixedLenByteArrayReader*>::type
            getColReaderPtr(parquet::ColumnReader* pColReader);

            /**
             * @brief print
             *            打印出Parquet的信息
             * @param stream
             *            打印信息到的流对象,如果是cout即在屏幕上打印
             * @param selectedColumnIndexs
             *            选择的列索引,如果传入的list为空, 那么默认为全部打印
             * @param isPrintValue
             *             是否需要打印出每一列的值
             *
             * 注意:
             *         如果metaData为null, 将会抛出异常
             */
            void printSchema(std::ostream& stream, const std::list<int>& selectedColumnIndexs, bool isPrintValue);
            void printSchema(std::ostream &stream, bool isPrintValue);

            /**
             * 和上面的实例函数功能一样, 不过是可以不用创建Parquet实例, 但是必须给出Parquet文件的路径
             */
            static void printSchema(std::string filePath, std::ostream &stream, bool isPrintValue);
            static void printSchema(std::string filePath, std::ostream& stream, const std::list<int>& selectedColumnIndexs, bool isPrintValue);

            /**
              *以下几个函数封装了ParquetFileReader的一些属性
              */

            /**
             *获取到metadata指针
             */
            const parquet::FileMetaData* fileMetadataPtr();
            /**
             *总共行数
             */
            int64_t rows();
            /**
             * 总共的rowGroup数量
             */
            int64_t rowGroups();
            /**
             * 总共的列数
             */
            int64_t columns();
            /**
            * @brief columnType
            *              获取第columnIndex索引位置的列类型
            * @param index
            *               列的索引位置
            * @return
            *               列类型, 类型由Parquet的Type定义
            *
            * 注意:
            *           当索引超出最大的列数时, 会抛出异常
            */
            parquet::Type::type  columnType(uint columnIndex);
            /**
            * @brief byteSizeOfRowGroup
            *            获取指定索引的rowGroup的大小, 以byte为单位
            * @param rowGroupIndex
            *            rowGroup的索引
            * @return
            *            指定索引的rowGroup的大小
            *
            * 注意:
            *           当索引超出最大的rowGroup数时, 会抛出异常
            */
            int64_t byteSizeOfRowGroup(uint rowGroupIndex);
            /**
            * @brief rowsOfRowGroup
            *             获取指定索引的rowGroup的行数
            * @param rowGroupIndex
            *               rowGroup的索引
            * @return
            *               指定索引的rowGroup的行数
            *
            *   注意:
            *           当索引超出最大的rowGroup数时, 会抛出异常
            */
            int64_t rowsOfRowGroup(uint rowGroupIndex);


            //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

            //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //write functions

            /**
             * @brief setupSchema
             *             配置Schema, 即Parquet的数据结构
             * @param fields
             *              每一列字段的vector
             *
             * 注意:
             *          以表的创建工作交给外部处理, 只是调用下setupSchema传入到Parquet对象中
             */
            void setupSchema( std::vector<parquet::schema::NodePtr>& fields);

            /**
            * @brief writeBatchedColumnsToRowGroup
            *              一次性写一个RowGroup数量的列到RowGroup
            * @param columnVector
            *               用于存储查询结果的columnVector结构
            * @param pRowGroupWriter
            *               读取每一个RowGroup的数据结构
            * @param currentRowIndex
            *               当前的行索引
            * @param rowsOfGroup
            *               每一个Group的行数
            *
            * 注意:
            *        原则上rowsOfGroup可以从Parquet::rowsPerRowGroup()获取, 但是实际写文件中, 写入的总行数不一定是rowsPerRowGroup的整数倍, 最后一个Group的行数
            *        会小于rowsPerRowGroup, 所有rowsOfGroup作为输入参数灵活设置
            */
            template<typename T>
            typename std::enable_if<std::is_same<T, int>::value ||
                    std::is_same<T, int64_t>::value ||
                    std::is_same<T, float>::value ||
                    std::is_same<T, double>::value ||
                    std::is_same<T, bool>::value ||
                    std::is_same<T, parquet::ByteArray>::value ||
                    std::is_same<T, parquet::Int96>::value>::type
           writeBatchedColumnsToRowGroup(
                    ColumnVector<T> &columnVector,
                    parquet::RowGroupWriter* pRowGroupWriter,
                    uint currentRowIndex,
                    uint rowsOfGroup);

            /**
             * @brief writeToFilePath
             *             写一个Parquet对象到文件(序列化)
             * @param filePath
             *              文件路径
             * @param codecType
             *              压缩格式
             * @param fields
             *              Schema字段vector
             *
             * 注意:
             *          1.在codecType中使用LZO会出错, 这里进行了异常保护
             *          2.resVals顺序需要和schema一一对应, 否则会抛出异常
             */
            template<class... Args>
            void writeToFilePath(
                    std::string& filePath,
                    parquet::Compression::type codecType,
                    uint rowCnt,
                    Args&& ... resVals);


            /**
             *以下所有的getColWriterPtr重载模板函数都是为了获取到正确的ColumnWriter指针使用的
             */

            template<typename T>//bool
            typename std::enable_if<std::is_same<T, bool>::value, parquet::BoolWriter*>::type
            getColWriterPtr(parquet::ColumnWriter* pColWriter);

            template<typename T>//Int32
            typename std::enable_if<std::is_same<T, int>::value, parquet::Int32Writer*>::type
            getColWriterPtr(parquet::ColumnWriter* pColWriter);

            template<typename T>//Int64
            typename std::enable_if<std::is_same<T, int64_t>::value, parquet::Int64Writer*>::type
            getColWriterPtr(parquet::ColumnWriter* pColWriter);

            template<typename T>//Int96
            typename std::enable_if<std::is_same<T, parquet::Int96>::value, parquet::Int96Writer*>::type
            getColWriterPtr(parquet::ColumnWriter* pColWriter);

            template<typename T>//float
            typename std::enable_if<std::is_same<T, float>::value, parquet::FloatWriter*>::type
            getColWriterPtr(parquet::ColumnWriter* pColWriter);

            template<typename T>//double
            typename std::enable_if<std::is_same<T, double>::value, parquet::DoubleWriter*>::type
            getColWriterPtr(parquet::ColumnWriter* pColWriter);

            template<typename T>//ByteArray
            typename std::enable_if<std::is_same<T, parquet::ByteArray>::value, parquet::ByteArrayWriter*>::type
            getColWriterPtr(parquet::ColumnWriter* pColWriter);

            template<typename T>//FixedLenByteArray
            typename std::enable_if<std::is_same<T, parquet::FixedLenByteArray>::value, parquet::FixedLenByteArrayWriter*>::type
            getColWriterPtr(parquet::ColumnWriter* pColWriter);

            //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        private:

          //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //member variant

            //parquet的读取类
            std::unique_ptr<parquet::ParquetFileReader> m_reader;

            //节点指针
            parquet::schema::NodePtr m_nodePtr;
            //schema描述器
            parquet::SchemaDescriptor m_schemaDescriptor;
            //parquet的写类
            std::unique_ptr<parquet::ParquetFileWriter> m_writer;

            //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        };

        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //definition of template functions

        //>>>-------------------------------------------------------------------------------------------------------------------------------------
        //1. sub class definition of template functions

        template <class T>
        Parquet::ColumnVector<T>::ColumnVector(uint valCnt):m_valCnt(valCnt)
       {
            init(valCnt);
        }
        template<class T>
        Parquet::ColumnVector<T>::ColumnVector()
        {

        }

        template <class T>
        void Parquet::ColumnVector<T>::init(uint valCnt)
        {
            m_values.resize(valCnt);
            m_definitionLevels.resize(valCnt);
            m_repetitionLevels.resize(valCnt);
            m_isNulls.resize(valCnt);
            m_valCnt = valCnt;
        }

        template<class T>
        T& Parquet::ColumnVector<T>::getVal(uint valIndex)
        {
            if(valIndex > this->m_valCnt - 1)
            {
                std::ostringstream message;
                message<<"Column Index["<<valIndex<<"is more than maxIndex["<<this->m_valCnt-1;
                std::string msg = message.str();

                THROW_EXCEPTION_WITH_OBJ(msg);
            }

            return this->values()[valIndex];
        }

        template<class T>
        void Parquet::ColumnVector<T>::dispose()
        {
            if(typeid(T) == typeid(parquet::ByteArray))
            {
                if(!this->m_isDisposed)
                {
                    this->m_isDisposed = true;
                    for(int i = 0; i<this->m_valCnt; ++i)
                    {
                        delete[] this->m_values[i].ptr;
                    }
                }
            }
        }

        //>>>-------------------------------------------------------------------------------------------------------------------------------------
        //2.read functions

        template<typename T>
        typename std::enable_if<std::is_same<T, int>::value ||
                                                std::is_same<T, int64_t>::value ||
                                                std::is_same<T, float>::value ||
                                                std::is_same<T, double>::value ||
                                                std::is_same<T, bool>::value ||
                                                std::is_same<T, parquet::ByteArray>::value ||
                                                std::is_same<T, parquet::Int96>::value>::type
        Parquet::readIndicatedColumn(
                ColumnVector<T>& columnVector, uint& columnIndex)
        {
            //>>>-------------------------------------------------------------------------------------------------------------------------------------
            //1.防呆验证
            if( nullptr == this->m_reader.get())
            {
                THROW_EXCEPTION_WITH_OBJ("Reader is null!");
            }

            uint columnCnt =  this->m_reader->metadata()->schema()->num_columns();
            if(columnIndex > columnCnt - 1)
            {
                std::ostringstream message;
                message<<"Column Index["<<columnIndex<<"is more than maxIndex["<<columnCnt-1;
                std::string msg = message.str();

                THROW_EXCEPTION_WITH_OBJ(msg);
            }

            uint rowCnt = this->m_reader->metadata()->num_rows();
            if(rowCnt==0)//行数为0的话, 直接退出
            {
                return;
            }
            else
            {
                if(columnVector.valCnt()!=rowCnt)
                {
                    columnVector.init(rowCnt);
                }
            }

            //>>>-------------------------------------------------------------------------------------------------------------------------------------
            //2.开始读取
            int rowGroupCnt = this->m_reader->metadata()->num_row_groups();

            for(int rowGroupIndex = 0; rowGroupIndex < rowGroupCnt; ++rowGroupIndex)//遍历每一个RowGroup
            {
                auto groupReader = this->m_reader->RowGroup(rowGroupIndex);
                auto colReader = groupReader->Column(columnIndex);
                int rowsOfGroup = groupReader->metadata()->num_rows();

                //注意:这里不能使用switch...case...来判断pColumnReader的physical_type走那一条分支, 因为在编译期间, 编译器
                //不知道走哪一个case, 所以所有case都会编译过去, 而当类型T不符合时, 会发生编译出错, 所以只能使用enable_if进行条件编译

                for( int rowIndex = 0; rowIndex < rowsOfGroup; ++rowIndex)
                {
                    //注意, 这里本来可以使用ReadBatch一次性读取rowsOfGroup数量的值, 但是由于val可能为null, 所以会导致读取的
                    //val和ref_level, def_level长度不相等, 最后上层想要方便使用, 还是要逐条判断是否为null, 所以还不如一条一条记录
                    //读取, 一旦发现为null了, 直接置对应的vector为null即可

                    readIndicatedRowValue<T>(colReader,columnVector,rowIndex);
                }
            }

            //这个+操作主要是为了将columnIndex指向当前读取的列中的下一列, 主要用于在反复调用该函数的时候,
            //只需要初始化columnIndex之后, 就可以依次把所有往后的所有列读取
            columnIndex++;
        }

        template<typename T>
        typename std::enable_if<std::is_same<T, int>::value ||
                                                std::is_same<T, int64_t>::value ||
                                                std::is_same<T, float>::value ||
                                                std::is_same<T, double>::value ||
                                                std::is_same<T, parquet::Int96>::value>::type//除了bool和ByteArray以外的所有类型
        Parquet:: readIndicatedRowValue(std::shared_ptr<parquet::ColumnReader>& colReader, ColumnVector<T>& columnVector, uint rowIndex)
        {
            int64_t foundCnt;

            /*问题:如果T的类型为ByteArray的话, columnVector.values()[rowIndex]在括号范围内被正确赋值,
                       一旦除了这个括号范围, 指针就被销毁了,再也获取不到正确的值,怀疑这在parquet-cpp中某一次被释放了*/
            this->getColReaderPtr<T>(colReader.get())->ReadBatch(
                        1,
                        &columnVector.definitionLevels()[rowIndex],
                        &columnVector.repetitionLevels()[rowIndex],
                        &columnVector.values()[rowIndex],
                        &foundCnt);

             columnVector.isNulls()[rowIndex] = (foundCnt == 0);
        }

        template<typename T>
        typename std::enable_if<std::is_same<T, bool>::value>::type//bool类型需要单独处理
        Parquet::readIndicatedRowValue(std::shared_ptr<parquet::ColumnReader>& colReader, ColumnVector<T>& columnVector, uint rowIndex)
        {
            int64_t foundCnt;
            bool value;
            this->getColReaderPtr<T>(colReader.get())->ReadBatch(
                        1,
                        &columnVector.definitionLevels()[rowIndex],
                        &columnVector.repetitionLevels()[rowIndex],
                        &value,//注意, 这里一定要用一个临时变量val来接受下, 再传入columnVector的values, 否则的话在类型为bool的情况下, 会编译不通过
                        &foundCnt);

            columnVector.values()[rowIndex] = value;
            columnVector.isNulls()[rowIndex] = (foundCnt == 0);
        }

        template<typename T>
        typename std::enable_if<std::is_same<T, parquet::ByteArray>::value>::type//ByteArray需要单独处理
        Parquet::readIndicatedRowValue(  std::shared_ptr<parquet::ColumnReader>& colReader, ColumnVector<T>& columnVector, uint rowIndex)
        {
            int64_t foundCnt;
            parquet::ByteArray value;
            this->getColReaderPtr<T>(colReader.get())->ReadBatch(
                        1,
                        &columnVector.definitionLevels()[rowIndex],
                        &columnVector.repetitionLevels()[rowIndex],
                        &value,
                        &foundCnt);

            /*注意:
                       1.这里必须重新new一个byte数组, 否则的话一旦出了这个花括号,value原来的数据会被释放
                        这里还需要注意的是,一旦new的新数组, 在外部用完了需要手动释放
                       2.这里的深复制代码不能写在ByteArray中, 因为一旦写了深复制, 在ByteArray中就需要添加相应的销毁指针代码,
                         第一是需要考虑避免重复释放的问题,
                         第二也会影响效率
               */

            columnVector.values()[rowIndex].ptr   = new uint8_t[value.len];
            columnVector.values()[rowIndex].len =  value.len;
            memcpy(const_cast<uint8_t*>(columnVector.values()[rowIndex].ptr), value.ptr, value.len);

            columnVector.isNulls()[rowIndex] = (foundCnt == 0);
        }

        template<class ...Args>
        void Parquet::readAllColumns(Args&&... resVals)
        {
            //>>>-------------------------------------------------------------------------------------------------------------------------------------
            //1.防呆验证
            if(nullptr == this->m_reader.get())//文件必须存在
            {
                THROW_EXCEPTION_WITH_OBJ("Reader is nullptr!");
            }


            //>>>-------------------------------------------------------------------------------------------------------------------------------------
            //2.开始读取
            uint colIndex{0};
            //用于可变参数模板的展开
            std::initializer_list<int> { (readIndicatedColumn(std::forward<Args>(resVals), colIndex),0) ... };
        }

        template<typename T>//bool
        typename std::enable_if<std::is_same<T,bool>::value, parquet::BoolReader*>::type
         Parquet::getColReaderPtr(parquet::ColumnReader* pColReader)
        {
            if(parquet::Type::BOOLEAN == pColReader->descr()->physical_type())
                return  reinterpret_cast<parquet::BoolReader*>(pColReader);
            else
                return nullptr;
        }

        template<typename T>//Int32
        typename std::enable_if<std::is_same<T,int>::value, parquet::Int32Reader*>::type
         Parquet::getColReaderPtr(parquet::ColumnReader* pColReader)
        {
            if(parquet::Type::INT32 == pColReader->descr()->physical_type())
                return  reinterpret_cast<parquet::Int32Reader*>(pColReader);
            else
                return nullptr;
        }

        template<typename T>//Int64
        typename std::enable_if<std::is_same<T,int64_t>::value, parquet::Int64Reader*>::type
         Parquet::getColReaderPtr(parquet::ColumnReader* pColReader)
        {
            if(parquet::Type::INT64 == pColReader->descr()->physical_type())
                return  reinterpret_cast<parquet::Int64Reader*>(pColReader);
            else
                return nullptr;
        }

        template<typename T>//Int96
        typename std::enable_if<std::is_same<T,parquet::Int96>::value, parquet::Int96Reader*>::type
         Parquet::getColReaderPtr(parquet::ColumnReader* pColReader)
        {
            if(parquet::Type::INT96 == pColReader->descr()->physical_type())
                return  reinterpret_cast<parquet::Int96Reader*>(pColReader);
            else
                return nullptr;
        }

        template<typename T>//float
        typename std::enable_if<std::is_same<T,float>::value, parquet::FloatReader*>::type
         Parquet::getColReaderPtr(parquet::ColumnReader* pColReader)
        {
            if(parquet::Type::FLOAT == pColReader->descr()->physical_type())
                return  reinterpret_cast<parquet::FloatReader*>(pColReader);
            else
                return nullptr;
        }

        template<typename T>//double
        typename std::enable_if<std::is_same<T,double>::value, parquet::DoubleReader*>::type
         Parquet::getColReaderPtr(parquet::ColumnReader* pColReader)
        {
            if(parquet::Type::DOUBLE == pColReader->descr()->physical_type())
                return  reinterpret_cast<parquet::DoubleReader*>(pColReader);
            else
                return nullptr;
        }

        template<typename T>//ByteArray
        typename std::enable_if<std::is_same<T, parquet::ByteArray>::value, parquet::ByteArrayReader*>::type
         Parquet::getColReaderPtr(parquet::ColumnReader* pColReader)
        {
            if(parquet::Type::BYTE_ARRAY == pColReader->descr()->physical_type())
                return  reinterpret_cast<parquet::ByteArrayReader*>(pColReader);
            else
                return nullptr;
        }

        template<typename T>//FixedLenByteArray
        typename std::enable_if<std::is_same<T, parquet::FixedLenByteArray>::value, parquet::FixedLenByteArrayReader*>::type
         Parquet::getColReaderPtr(parquet::ColumnReader* pColReader)
        {
            if(parquet::Type::BYTE_ARRAY == pColReader->descr()->physical_type())
                return  reinterpret_cast<parquet::FixedLenByteArrayReader*>(pColReader);
            else
                return nullptr;
        }


        //>>>-------------------------------------------------------------------------------------------------------------------------------------
        //3. write functions

        template<class... Args>
        void Parquet::writeToFilePath(
                std::string &filePath,
                parquet::Compression::type codecType,
                uint rowCnt,
                Args && ... resVals)
        {
            try
            {
                //>>>-------------------------------------------------------------------------------------------------------------------------------------
                //1.防呆验证

                //判断文件的上层目录是否存在, 如果不存在就抛出异常
                FileInfo fileInfo(filePath);
                auto parentFolder = fileInfo.getParentPath();
                if(!FileInfo::exists(parentFolder))
                {
                    THROW_EXCEPTION_WITH_OBJ("Parent Folder["+ filePath + "] is not existed!" );
                }

                //如果使用LZO会出错, 这里进行异常保护
                if( codecType == parquet::Compression::type::LZO)
                {
                     THROW_EXCEPTION_WITH_OBJ("LZO is not supported by parquet-cpp!" );
                }

                std::shared_ptr<::arrow::io::FileOutputStream> outFile;
                ::arrow::io:: FileOutputStream::Open(filePath,&outFile);//确认这行代码是否必要

                auto gNode =  std::static_pointer_cast<parquet::schema::GroupNode>(this->m_nodePtr);
                parquet::WriterProperties::Builder builder;
                int columnCnt = this->m_schemaDescriptor.num_columns();

                //>>>-------------------------------------------------------------------------------------------------------------------------------------
                //2.压缩
                if(codecType != parquet::Compression::type::UNCOMPRESSED)//只有在需要压缩的情况才压缩
                {
                    for(int i = 0; i < columnCnt; ++i)
                    {
                        builder.compression(this->m_schemaDescriptor.Column(i)->name(), codecType);
                    }
                }
                std::shared_ptr<parquet::WriterProperties> writerProperties = builder.build();

                this->m_writer = parquet::ParquetFileWriter::Open(outFile, gNode,writerProperties ) ;

                //>>>-------------------------------------------------------------------------------------------------------------------------------------
                //3. 依次写入所有列

                uint rowsPerRowGroup{Parquet::rowsPerRowGroup()};
                uint rowGroupCnt {0};
                uint rowCntLastGroup { rowCnt % rowsPerRowGroup};
                uint rowCurrentIndex{0};

                if( rowsPerRowGroup > rowCnt )//当rowsPerRowGroup大于总行数时, 总共就1个RowGroup
                {
                    rowGroupCnt = 1;\

                    parquet::RowGroupWriter* pGroupWriter = this->m_writer->AppendRowGroup(rowCnt);
                    std::initializer_list<int> { (writeBatchedColumnsToRowGroup( std::forward<Args>(resVals), pGroupWriter,  rowCurrentIndex,  rowCnt), 0 ) ... };
                    pGroupWriter->Close();
                }
                else//当rowsPerRowGroup小与行数时, 至少有2个RowGroup
                {
                    if(rowCntLastGroup != 0)
                    {
                        rowGroupCnt = rowCnt / rowsPerRowGroup + 1;

                        //除去最后一个RowGroup, 其它RowGroup的行数都是rowsPerRowGroup
                        for(uint groupIndex = 0; groupIndex < rowGroupCnt - 1; ++groupIndex)
                        {
                            parquet::RowGroupWriter* pGroupWriter = this->m_writer->AppendRowGroup(rowsPerRowGroup);
                            std::initializer_list<int> { (writeBatchedColumnsToRowGroup( std::forward<Args>(resVals), pGroupWriter,  rowCurrentIndex,  rowsPerRowGroup), 0 ) ... };
                            rowCurrentIndex += rowsPerRowGroup;
                            pGroupWriter->Close();
                        }

                        //单独处理最后一个RowGroup
                        parquet::RowGroupWriter* pGroupWriter = this->m_writer->AppendRowGroup(rowCntLastGroup);
                        std::initializer_list<int> { (writeBatchedColumnsToRowGroup( std::forward<Args>(resVals), pGroupWriter,  rowCurrentIndex,  rowCntLastGroup), 0 ) ... };
                        pGroupWriter->Close();
                    }
                    else
                    {
                        rowGroupCnt = rowCnt / rowsPerRowGroup;

                        //刚好可以整除, 全部Group的行数都为rowsPerRowGroup
                        for(uint groupIndex = 0; groupIndex < rowGroupCnt; ++groupIndex)
                        {
                            parquet::RowGroupWriter* pGroupWriter = this->m_writer->AppendRowGroup(rowsPerRowGroup);
                            std::initializer_list<int> { (writeBatchedColumnsToRowGroup( std::forward<Args>(resVals), pGroupWriter,  rowCurrentIndex,  rowsPerRowGroup), 0 ) ... };
                            rowCurrentIndex += rowsPerRowGroup;
                            pGroupWriter->Close();
                        }
                    }
                }

                this->m_writer->Close();//不同与Reader, Writer需要手动Close
            }
            CATCH_THROW_EXCEPTION_WITH_OBJ("Failed to write to file ["+ filePath +"]");
        }

        template<typename T>
        typename std::enable_if<std::is_same<T, int>::value ||
                std::is_same<T, int64_t>::value ||
                std::is_same<T, float>::value ||
                std::is_same<T, double>::value ||
                std::is_same<T, bool>::value ||
                std::is_same<T, parquet::ByteArray>::value ||
                std::is_same<T, parquet::Int96>::value>::type
        Parquet::writeBatchedColumnsToRowGroup(
                ColumnVector<T> &columnVector,
                parquet::RowGroupWriter* pRowGroupWriter,
                uint currentRowIndex,
                uint rowsOfGroup)
        {
            auto columnWriter = pRowGroupWriter->NextColumn();

            this->getColWriterPtr<T>(columnWriter)->WriteBatch(
                        rowsOfGroup,
                        &columnVector.definitionLevels()[currentRowIndex],
                        &columnVector.repetitionLevels()[currentRowIndex],
                        &columnVector.values()[currentRowIndex]);
            columnWriter->Close();
        }

        template<typename T>//bool
        typename std::enable_if<std::is_same<T,bool>::value, parquet::BoolWriter*>::type
         Parquet::getColWriterPtr(parquet::ColumnWriter* pColWriter)
        {
            if(parquet::Type::BOOLEAN == pColWriter->descr()->physical_type())
                return  reinterpret_cast<parquet::BoolWriter*>(pColWriter);
            else
                return nullptr;
        }

        template<typename T>//Int32
        typename std::enable_if<std::is_same<T,int>::value, parquet::Int32Writer*>::type
         Parquet::getColWriterPtr(parquet::ColumnWriter* pColWriter)
        {
            if(parquet::Type::INT32 == pColWriter->descr()->physical_type())
                return  reinterpret_cast<parquet::Int32Writer*>(pColWriter);
            else
                return nullptr;
        }

        template<typename T>//Int64
        typename std::enable_if<std::is_same<T,int64_t>::value, parquet::Int64Writer*>::type
         Parquet::getColWriterPtr(parquet::ColumnWriter* pColWriter)
        {
            if(parquet::Type::INT64 == pColWriter->descr()->physical_type())
                return  reinterpret_cast<parquet::Int64Writer*>(pColWriter);
            else
                return nullptr;
        }

        template<typename T>//Int96
        typename std::enable_if<std::is_same<T,parquet::Int96>::value, parquet::Int96Writer*>::type
         Parquet::getColWriterPtr(parquet::ColumnWriter* pColWriter)
        {
            if(parquet::Type::INT96 == pColWriter->descr()->physical_type())
                return  reinterpret_cast<parquet::Int96Writer*>(pColWriter);
            else
                return nullptr;
        }

        template<typename T>//float
        typename std::enable_if<std::is_same<T,float>::value, parquet::FloatWriter*>::type
         Parquet::getColWriterPtr(parquet::ColumnWriter* pColWriter)
        {
            if(parquet::Type::FLOAT == pColWriter->descr()->physical_type())
                return  reinterpret_cast<parquet::FloatWriter*>(pColWriter);
            else
                return nullptr;
        }

        template<typename T>//double
        typename std::enable_if<std::is_same<T,double>::value, parquet::DoubleWriter*>::type
         Parquet::getColWriterPtr(parquet::ColumnWriter* pColWriter)
        {
            if(parquet::Type::DOUBLE == pColWriter->descr()->physical_type())
                return  reinterpret_cast<parquet::DoubleWriter*>(pColWriter);
            else
                return nullptr;
        }

        template<typename T>//ByteArray
        typename std::enable_if<std::is_same<T, parquet::ByteArray>::value, parquet::ByteArrayWriter*>::type
         Parquet::getColWriterPtr(parquet::ColumnWriter* pColWriter)
        {
            if(parquet::Type::BYTE_ARRAY == pColWriter->descr()->physical_type())
                return  reinterpret_cast<parquet::ByteArrayWriter*>(pColWriter);
            else
                return nullptr;
        }

        template<typename T>//FixedLenByteArray
        typename std::enable_if<std::is_same<T, parquet::FixedLenByteArray>::value, parquet::FixedLenByteArrayWriter*>::type
         Parquet::getColWriterPtr(parquet::ColumnWriter* pColWriter)
        {
            if(parquet::Type::BYTE_ARRAY == pColWriter->descr()->physical_type())
                return  reinterpret_cast<parquet::FixedLenByteArrayWriter*>(pColWriter);
            else
                return nullptr;
        }
    }//End of namespace Archive
}//End of namespace SSDK

#endif // PARQUET_HPP
