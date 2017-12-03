#ifndef PROTOCOLBUFFER_HPP
#define PROTOCOLBUFFER_HPP

#include <string>
#include <memory>
 #include <fstream>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "google/protobuf/descriptor.h"
#include "google/protobuf/compiler/importer.h"
#include "google/protobuf/message.h"
#include "google/protobuf/descriptor.pb.h"
#include "google/protobuf/compiler/plugin.pb.h"
#include "google/protobuf/dynamic_message.h"

#include "fileinfo.hpp"
#include "Exception/customexception.hpp"

#ifndef O_BINARY
 #define O_BINARY 0
 #endif

namespace SSDK
{
    namespace Archive
    {
        /**
         *  @brief ProtocolBuffer
         *
         *  @author rime
         *  @version 1.00 2017-06-05 rime
         *                note:create it
         *
         * 主要参考:
         *          <<Google Protocol Buffer 的使用和原理>>
         *                  https://www.ibm.com/developerworks/cn/linux/l-cn-gpb/
         *
         * 注意:
         *          ProtocolBuf数据文件分为静态编译和动态编译两种:
         *          1.静态编译, 基于定义好的proto文件直接生成头文件和源文件, 后续代码直接使用这两个文件
         *          2.动态编译, 没有已经编译好的头文件和源文件, 直接由输入的proto文件路径动态生成,为了调用方便, 所以封装了该ProtocolBuffer类
         *          3.一个ProtocolBuf对应一个Proto文件, 所有读取都是实例成员函数, 而protoc对应的对象的创建和读写都是静态函数
         *          4.ProtocolBuf主要还是用于单个对象的序列化(用于通讯协议、数据存储等领域的语言无关、平台无关、可扩展的序列化结构数据格式),
         *             所以这里没有对于集合对象的序列化(源码的Demo中也没有找到), 后续实在有需要了再添加
         *
         * 具体说明见具体见Doc/ProtocolBuf的使用说明
         *
         */
        class ProtocolBuffer
        {
        public:

            //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //macro

            /**
              * 确认File Descriptor指针是否为空
              */
            #define CHECK_FILE_DESCRIPTION\
            {\
                if(nullptr == this->m_pFile)\
                {\
                    THROW_EXCEPTION_WITH_OBJ("Pointer of File descriptor is nullptr!");\
                }\
            }

            /**
              * 确认Descriptor指针是否为空
              */
            #define CHECK_DESCRIPTION(pDescroption)\
            {\
                if(nullptr == pDescroption)\
                {\
                    THROW_EXCEPTION_WITHOUT_OBJ("Pointer of  descriptor is nullptr!");\
                }\
            }

            /**
              * 设置字段之前的防呆操作
              *         由于该逻辑在所有类型的值设置前都需要, 为了节约效率及代码简洁,使用宏定义
              */
            #define CHECK_BEFORE_SET_OR_GET_VALUE(pMessage, pDescroption)\
            { \
                if(nullptr == pMessage)\
               {\
                    THROW_EXCEPTION_WITHOUT_OBJ("Pointer of Message is nullptr!");\
                }\
                \
                CHECK_DESCRIPTION(pDescroption)\
            }

            //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


            //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //constructor & deconstructor

            /**
             * @brief ProtocolBuffer
             *              默认构造函数
             */
            ProtocolBuffer();
            /**
             * @brief ProtocolBuffer
             *              从指定文件加载一个类型
             * @param filePath
             *               protoc文件路径
             */
            ProtocolBuffer(const std::string& filePath);
            virtual ~ProtocolBuffer();

            //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

            //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //read functions

            /**
             * @brief loadTypeFromFilePath
             *              从文件动态加载一个类型
             * @param filePath
             *              protoc文件路径
             *
             * 注意:
             *               这里加载的是一个Protoc类型, 而不是一个Message对象
             */
            void loadTypeFromFilePath(const std::string& filePath);

            /**
             * @brief findMessageTypeByName
             *              根据Message的名称获取到Message类型的描述器
             * @param name
             *              Message的对象
             * @return
             *              返回的Message的描述器指针
             *
             *  注意:
             *              这里返回的Descriptor不需要考虑内存释放问题, 因为protocolBuffer会内部会进行释放
             */
            const google::protobuf::Descriptor* findMessageTypeByName(const std::string& name)const;

            /**
             * @brief fieldCount
             *              获取字段的数量
             * @param pDescriptor
             *              字段所有在Message的描述器指针对象
             * @return
             *
             * 注意:
             *          返回字段数量
             */
            static uint fieldCount(const google::protobuf::Descriptor* pDescriptor);

            /**
             * @brief findFindFieldByName
             *              根据字段的名称获取到字段的描述器类型
             * @param pDescriptor
             *               字段所有在Message的描述器指针对象
             * @param name
             *               字段的名称
             * @return
             *              返回的字段描述器指针
             *
             * 注意:
             *           1.这里返回的Descriptor不需要考虑内存释放问题, 因为protocolBuffer会内部会进行释放
             *           2.如果没有找到, 返回null
             */
            static const google::protobuf::FieldDescriptor* findFieldByName(
                    const google::protobuf::Descriptor* pDescriptor,
                    const std::string& name);

            /**
             * @brief findFindFieldByName
             *              根据枚举的名称获取到枚举的描述类型
             * @param pDescriptor
             *               枚举所有在Message的描述指针对象
             * @param name
             *               枚举的名称
             * @return
             *              返回的枚举的描述器指针
             *
             * 注意:
             *           1.这里返回的Descriptor不需要考虑内存释放问题, 因为protocolBuffer会内部会进行释放
             *           2.如果没有找到, 返回null
             */
            static const google::protobuf::EnumDescriptor* findEnumByName(
                    const google::protobuf::Descriptor* pDescriptor,
                    const std::string& name);

            /**
             * @brief debugString
             *              返回描述器的调式信息
             * @param pDescriptor
             *              描述器指针
             * @return
             *              描述器的调式信息
             */
           static std::string debugString(const google::protobuf::Descriptor* pDescriptor);

           /**
             * @brief debugString
             *             返回该Protoc类型的调式信息
             * @return
             *             描述器的调式信息
             */
            std::string debugString();

            //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

            //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //serializaion & deserialization functions

            /**
             * @brief readMessageFromFilePath
             *              从文件动态加载一个Message
             * @param filePath
             *              指定的文件路径
             *
             * 注意:
             *          路径的不存在时, 会抛出异常
             */
            static void  deserializeFromFilePath(google::protobuf::Message& message, const std::string& filePath);
            /**
             * @brief parseFromStream
             *             从输入流中解析一个Message对象
             * @param message
             *              需要更新的Message对象
             * @param inputStream
             *              输入流
             */
            static void  parseFromStream(google::protobuf::Message& message,  std::istream& inputStream);

            /**
             * @brief serializeToFilePath
             *             写一个Message对象到指定路径
             * @param message
             *              需要写入的Message对象
             * @param filePath
             *              写入的文件路径
             *
             * 注意:
             *          路径的目录不存在时, 会抛出异常
             */
            static void  serializeToFilePath(google::protobuf::Message& message,  const std::string& filePath);
            /**
             * @brief serializeToStream
             *              写一个Message对象到输出流
             * @param message
             *              需要写入的Message对象
             * @param output
             *              写入的输出流对象
             */
            static void  serializeToStream(google::protobuf::Message& message,  std::ostream& outputStream);

            //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

            //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //create function

            /**
             * @brief createMessageInstance
             *             创建一个新的Message实例
             * @param name
             *              Message的名称
             * @return 新创建的Message对象指针
             *
             * 注意:
             *          返回值必须要用智能指针封装, 否则会产生内存泄露
             */
            std::unique_ptr<google::protobuf::Message> createMessageInstance(const std::string& name);

            //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


            //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //set functions

            /**
             *设置字段, 支持int32_t, int64_t,uint32_t,uint64_t,float,double,bool,string
             */

            /**
             * @brief setValue
             *             设置字段的值
             * @param message
             *              对应的message类型的智能指针对象
             * @param pFieldDescriptor
             *              对应需要设置值的字段描述器的指针
             * @param value
             *              设置的值
             *
             * 注意:
             *          1.由于int32_t, int64_t, uint32_t, uint64_t,float,double,bool这些c++的内置类型默认就可以相互转换,所以这些类型的重载
             *             函数可以写在一起, 在函数内部再作进一步的类型匹配验证; 而string无法和上述类型转换, 所以单独重载一个函数
             *          2.枚举Enum在ProtocBuf中用EnumValueDescriptor表示, 但是由于Enum内部都是使用了int(C++标准的枚举也是这样),
             *             为了更好的封装一致性, 这里直接接受int作为枚举值, 和int的区分通过FieldDescriptor中的CppType来实现
             */
            template <typename T>
            typename std::enable_if<std::is_same<T,  int32_t>::value ||
                std::is_same<T,  int64_t>::value ||
                std::is_same<T,  uint32_t>::value ||
                std::is_same<T,  uint64_t>::value ||
                std::is_same<T,  float>::value ||
                std::is_same<T,  double>::value ||
                std::is_same<T,  bool>::value>::type
            static setValue(
                    std::unique_ptr<google::protobuf::Message>& message,
                    const google::protobuf::FieldDescriptor* pFieldDescriptor,
                     T value);

            template <typename T>//这里仅仅接受string类型
            typename std::enable_if<std::is_same<T,  std::string>::value >::type
            static setValue(
                    std::unique_ptr<google::protobuf::Message>& message,
                    const google::protobuf::FieldDescriptor* pFieldDescriptor,
                    const T& value);

            //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

            //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //get functions

            /**
             * @brief getValue
             *             获取字段的值
             * @param message
             *              对应的message类型的对象智能指针对象
             * @param pFieldDescriptor
             *              对应需要设置值的字段描述器的指针
             * @return
             *              对应字段的值
             *
             * 注意:
             *          1.由于int32_t, int64_t, uint32_t, uint64_t,float,double,bool这些c++的内置类型默认就可以相互转换,所以这些类型的重载
             *             函数可以写在一起, 在函数内部再作进一步的类型匹配验证; 而string无法和上述类型转换, 所以单独重载一个函数
             *          2.枚举Enum在ProtocBuf中用EnumValueDescriptor表示, 但是由于Enum内部都是使用了int(C++标准的枚举也是这样),
             *             为了更好的封装一致性, 这里直接接受int作为枚举值, 和int的区分通过FieldDescriptor中的CppType来实现
             */
            template <typename T>
            typename std::enable_if<std::is_same<T,  int32_t>::value ||
                    std::is_same<T,  int64_t>::value ||
                    std::is_same<T,  uint32_t>::value ||
                    std::is_same<T,  uint64_t>::value ||
                    std::is_same<T,  float>::value ||
                    std::is_same<T,  double>::value ||
                    std::is_same<T,  bool>::value, T>::type
            static getValue(
                    std::unique_ptr<google::protobuf::Message>& message,
                    const google::protobuf::FieldDescriptor*  pFieldDescriptor);

            template <typename T>//这里仅仅接受string类型
            typename std::enable_if<std::is_same<T,  std::string>::value,  std::string>::type
            static getValue(
                    std::unique_ptr<google::protobuf::Message>& message,
                    const google::protobuf::FieldDescriptor*  pFieldDescriptor);

            //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        private:

            //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //variant member

            //proto的导入类
            std::unique_ptr<google::protobuf::compiler::Importer> m_importer;

            //protoc类型文件(也就是定义的数据结构)的描述器,该成员不需要手动释放
            const google::protobuf::FileDescriptor* m_pFile;

            //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


            //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //auxiliary function

            /**
              * @brief checkBeforeSet
              *             字段赋值之前的验证工作, 主要是判断模板参数的类型和值与protocBuf中字段中的类型是否对应
              * @param value
              *             当前字段的值
              * @param cppType
              *             当前字段对于的Cpp类型枚举
              * @return
              *             如果类型匹配,返回true;否则返回false
              *
              * 注意:
              *         1.这里的验证会根据value的类型, 值的大小和cppType综合判断,比如说cppType的类型是uint32, 但是value的
              *            类型时int32, 这种情况下只要 value的值在uint32的区间内还是满足要求的. 虽然原则上应该一一对应, 这里
              *            主要考虑到在调用setValue的时候直接输入一个值,也就是右值的时候使用方便
              */
             template <typename T>
             static void checkBeforeSet(T value, google::protobuf::FieldDescriptor::CppType cppType);

             /**
              * @brief checkBeforeGet
              *             字段获取之前的验证工作, 主要是判断模板参数的类型与protocBuf中字段中的类型是否对应
              * @param cppType
              *             当前字段对于的Cpp类型枚举
              * @return
              *             如果类型匹配,返回true;否则返回false
              *
              * 注意:
              *        这里和checkBeforeSet不同, 因为getValue不会存在获取右值的情况, 所以返回的类型只可以往范围大的方向
              * 转, 不能往方向小的方向转, 比如说cppType的类型是uint32, T的类型只能时uint32/uint64/int64, 而不能是int32.
              *         当然也可以在获取了实际对象的值之后再进行类型判断, 但是这样写的话一个是代码看上去臃肿, 因为需要设定不同类型的值先接受
              * 值,然后再根据值进行进一步的类型判断, 就像checkBeforeSet一样, 这种方法也不是严格的类型安全检查
              *
              */
             template <typename T>
             static void checkBeforeGet( google::protobuf::FieldDescriptor::CppType cppType);

            //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        };//End of ProtocolBuffer

        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //definition template function out of class

        //>>>-------------------------------------------------------------------------------------------------------------------------------------
        //1. set value functions

        template <typename T>
        typename std::enable_if<std::is_same<T,  int32_t>::value ||
            std::is_same<T,  int64_t>::value ||
            std::is_same<T,  uint32_t>::value ||
            std::is_same<T,  uint64_t>::value ||
            std::is_same<T,  float>::value ||
            std::is_same<T,  double>::value ||
            std::is_same<T,  bool>::value>::type
        ProtocolBuffer:: setValue(
                std::unique_ptr<google::protobuf::Message>& message,
                const google::protobuf::FieldDescriptor* pFieldDescriptor,
                T value)
        {
            CHECK_BEFORE_SET_OR_GET_VALUE(message.get(), pFieldDescriptor);

            //模板参数的类型与protoc中字段的类型能够一一对应
            auto cppType = pFieldDescriptor->cpp_type();
            checkBeforeSet(value, cppType);

            const google::protobuf::Reflection*  pReflection = message.get()->GetReflection();
            switch (cppType)
            {
            case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
                pReflection->SetInt32(message.get(),pFieldDescriptor,value);
                break;

            case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
                pReflection->SetInt64(message.get(),pFieldDescriptor,value);
                break;

            case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
                pReflection->SetUInt32(message.get(),pFieldDescriptor,value);
                break;

            case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
                pReflection->SetUInt64(message.get(),pFieldDescriptor,value);
                break;

            case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
                pReflection->SetFloat(message.get(),pFieldDescriptor,value);
                break;

            case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
                pReflection->SetDouble(message.get(),pFieldDescriptor,value);
                break;

            case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
                pReflection->SetBool(message.get(),pFieldDescriptor,value);
                break;

            case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
            {
                const google::protobuf::EnumValueDescriptor* pEnumValue =
                        pFieldDescriptor->enum_type()->FindValueByNumber(value);
                pReflection->SetEnum(message.get(),pFieldDescriptor,pEnumValue);
            }
                break;

            default:
                break;//注意:这里如果类型不支持的话, 已经不需要再抛出异常了, 因为类型验证在前面已经做过了
            }
        }

        template <typename T>
        typename std::enable_if<std::is_same<T,  std::string>::value >::type
        ProtocolBuffer:: setValue(
                std::unique_ptr<google::protobuf::Message>& message,
                const google::protobuf::FieldDescriptor* pFieldDescriptor,
                const T& value)
        {
                CHECK_BEFORE_SET_OR_GET_VALUE(message.get(), pFieldDescriptor);

                const google::protobuf::Reflection*  pReflection = message->GetReflection();
                pReflection->SetString(message.get(),pFieldDescriptor,value);
        }

        //>>>-------------------------------------------------------------------------------------------------------------------------------------
        //2. get value functions

        template <typename T>
        typename std::enable_if<std::is_same<T,  int32_t>::value ||
        std::is_same<T,  int64_t>::value ||
        std::is_same<T,  uint32_t>::value ||
        std::is_same<T,  uint64_t>::value ||
        std::is_same<T,  float>::value ||
        std::is_same<T,  double>::value ||
        std::is_same<T,  bool>::value, T>::type
        ProtocolBuffer:: getValue(
                std::unique_ptr<google::protobuf::Message>& message,
                const google::protobuf::FieldDescriptor*  pFieldDescriptor)
        {
            CHECK_BEFORE_SET_OR_GET_VALUE(message.get(), pFieldDescriptor);

            //模板参数的类型与protoc中字段的类型能够一一对应
            auto cppType = pFieldDescriptor->cpp_type();
            checkBeforeGet<T>(cppType);

            const google::protobuf::Reflection*  pReflection = message.get()->GetReflection();
            switch (pFieldDescriptor->cpp_type())
            {
            case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
                return pReflection->GetInt32(*message.get(), pFieldDescriptor);

            case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
                return pReflection->GetInt64(*message.get(), pFieldDescriptor);

            case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
                return pReflection->GetUInt32(*message.get(), pFieldDescriptor);

            case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
                return pReflection->GetUInt64(*message.get(), pFieldDescriptor);

            case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
                return pReflection->GetFloat(*message.get(), pFieldDescriptor);

            case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
                return pReflection->GetDouble(*message.get(), pFieldDescriptor);

            case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
                return pReflection->GetBool(*message.get(), pFieldDescriptor);

            case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
                return pReflection->GetEnum(*message.get(), pFieldDescriptor)->number();

            default:
                std::ostringstream message;\
                message<<"Type:["<<cppType<<"] is not matched now!";
                 std::string msg = message.str();\
                THROW_EXCEPTION_WITHOUT_OBJ(msg);
            }
        }

        template <typename T>
        typename std::enable_if<std::is_same<T,  std::string>::value, std::string>::type
        ProtocolBuffer:: getValue(
                std::unique_ptr<google::protobuf::Message>& message,
                const google::protobuf::FieldDescriptor*  pFieldDescriptor)
        {
            CHECK_BEFORE_SET_OR_GET_VALUE(message.get(), pFieldDescriptor);
             const google::protobuf::Reflection*  pReflection = message.get()->GetReflection();

             return pReflection->GetString(*message.get(),pFieldDescriptor);
        }

        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        //>>>-------------------------------------------------------------------------------------------------------------------------------------
        //3. auxiliary function

        template <typename T>
        void ProtocolBuffer:: checkBeforeSet(T value, google::protobuf::FieldDescriptor::CppType cppType)
        {
            bool isMatched{false};
            switch (cppType)
            {
            case google::protobuf::FieldDescriptor::CPPTYPE_INT32://[INT32_MIN, INT32_MAX]
                isMatched = (typeid(T) == typeid(uint32_t) &&  value <= std::numeric_limits<int32_t>::max() ) ||
                           typeid(T) == typeid(int32_t) ||
                           (typeid(T) == typeid(uint64_t) &&  value <= std::numeric_limits<int32_t>::max()  && value >= std::numeric_limits<int32_t>::min() ) ||
                           (typeid(T) == typeid(int64_t) &&  value <= std::numeric_limits<int32_t>::max()  && value >= std::numeric_limits<int32_t>::min() );
                 break;

            case google::protobuf::FieldDescriptor::CPPTYPE_INT64://[INT64_MIN, INT64_MAX]
                isMatched = typeid(T) == typeid(uint32_t)  ||
                          (typeid(T) == typeid(uint64_t)  && value <=  std::numeric_limits<int64_t>::max())||
                          typeid(T) == typeid(int32_t) ||
                          typeid(T) == typeid(int64_t) ;
                 break;

            case google::protobuf::FieldDescriptor::CPPTYPE_UINT32://[0, UINT32_MAX]
                isMatched = typeid(T) == typeid(uint32_t) ||
                          (typeid(T) == typeid(int32_t) && value >= 0) ||
                          (typeid(T) == typeid(uint64_t) && value >= 0 && value <=  std::numeric_limits<uint32_t>::max()) ||
                          (typeid(T) == typeid(int64_t) && value >= 0 && value <= std::numeric_limits<uint32_t>::max());
                 break;

            case google::protobuf::FieldDescriptor::CPPTYPE_UINT64://
                isMatched = typeid(T) == typeid(uint64_t) ||
                           typeid(T) == typeid(uint32_t)||
                           (typeid(T) == typeid(int32_t) && value >= 0)||
                           (typeid(T) == typeid(int64_t) && value >= 0);
                 break;

            case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
                isMatched = typeid(T) == typeid(float) ||
                          (typeid(T) == typeid(double) && value >=  std::numeric_limits<float>::min() && value <=  std::numeric_limits<float>::max() );
                 break;

            case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
                isMatched =  typeid(T) == typeid(double) ||
                           typeid(T) == typeid(float);
                 break;

            case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
                 isMatched = typeid(T) == typeid(bool) ;
                  break;

            case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
                 isMatched = typeid(T) == typeid(const google::protobuf::EnumValueDescriptor*)||
                           typeid(T) == typeid(int);
                  break;

            case  google::protobuf::FieldDescriptor::CPPTYPE_STRING:
                isMatched = typeid(T) == typeid(std::string);
                 break;

            default:
                isMatched = false;
                 break;
            }

            if(!isMatched)
            {
                std::ostringstream message;\
                message<<"Type:["<<cppType<<"] is not matched now!";
                 std::string msg = message.str();\
                THROW_EXCEPTION_WITHOUT_OBJ(msg);
            }
        }

        template <typename T>
        void ProtocolBuffer:: checkBeforeGet( google::protobuf::FieldDescriptor::CppType cppType)
        {
             bool isMatched{false};
            switch (cppType)
            {
            case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
                isMatched = typeid(T) == typeid(int32_t) ||
                                     typeid(T) == typeid(uint64_t) ||
                                     typeid(T) == typeid(int64_t);
                break;

            case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
                isMatched =  typeid(T) == typeid(int64_t) ;

            case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
                isMatched = typeid(T) == typeid(uint32_t) ||
                          typeid(T) == typeid(uint64_t) ||
                          typeid(T) == typeid(int64_t);
                 break;

            case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
                isMatched = typeid(T) == typeid(uint64_t);
                 break;

            case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
                isMatched = typeid(T) == typeid(float) ||
                          (typeid(T) == typeid(double));
                 break;

            case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
                isMatched = typeid(T) == typeid(double) ||
                           typeid(T) == typeid(float);
                 break;

            case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
                 isMatched = typeid(T) == typeid(bool) ;
                  break;

            case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
                isMatched = typeid(T) == typeid(const google::protobuf::EnumValueDescriptor*)||
                           typeid(T) == typeid(int);
                 break;

            case  google::protobuf::FieldDescriptor::CPPTYPE_STRING:
                isMatched = typeid(T) == typeid(std::string);
                 break;

            default:
                isMatched = false;
                 break;
            }

            if(!isMatched)
            {
                std::ostringstream message;\
                message<<"Type:["<<cppType<<"] is not matched now!";
                 std::string msg = message.str();\
                THROW_EXCEPTION_WITHOUT_OBJ(msg);
            }
        }
    }//End of namespace Archive
}//End of namespace SSDK



#endif // PROTOCOLBUFFER_HPP
