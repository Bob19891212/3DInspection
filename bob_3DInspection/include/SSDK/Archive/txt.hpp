#ifndef TXT_HPP
#define TXT_HPP

#include <string>
#include <map>
#include <vector>
//#include <boost/any.hpp>

#include <QMap>

#include "metaenum.hpp"

namespace SSDK
{
    namespace Archive
    {
        class Txt
        {
        public:
            /**
             * @brief Delimiter
             * DSV(Delimiter Seperated Values)格式支持4种分隔符
             */
            SEnum(Delimiter,
                  BLANK,//空格
                  SEMICOLON,//；
                  COMMA,//，
                  TAB,//制表符
                  );

            /**
             * 代表每一列的数据结构，可以支持任意类型，支持集合类型
             * 当每一列有多个参数时，因为参数可以为任意类型，所有统一先放在一个QString的vector中，到具体的数据结构中再负责转化
             *
             * 注意:
             *      1.当每一列参数中可能会包含多个参数，如果参数类型就是string类型的话，可能会包含分隔符，解决的办法是加一个转义符‘\',
             *        toQString函数就负责将m_val具体内容转化为QString，该加‘\’的就加
             *      2.当从txt读入时，需要处理转移符，通过fromQString函数处理
             */
            struct AttributeValue
            {
                Delimiter m_delimiter{ Delimiter::COMMA};//每列属性参数的分隔符，默认为“,”
                std::vector<QString> m_val;//每列属性的值，可以支持多个参数,多个类型

                bool isMultiValue() { m_val.size() > 1; }//该属性是否为多个参数

                QString toQString() const;//转为一个QString

                void fromQString(const QString& contextQString);//由一个QString转化为来,
            };

            Txt();
            virtual ~Txt();

            Delimiter attributeDelimiter(){return m_attributeDelimiter;}
            void setAttributeDelimiter(Delimiter delimiter){m_attributeDelimiter = delimiter;}

            /**
             *这里需要确认下T能否接受数组类型，如int[],double[]等，好像直接使用T[]会编译出错
             */
            template<typename T>
            void writeDataArrayToFilePath(const QString& filePath,Delimiter delimiter, const T& dataArray);

            template<typename T>
            const T& readDataArrayFromFilePath(const QString& filePath, Delimiter delimiter);

            void writeAttributesToFilePath(const QString& filePath, const QMap<QString,AttributeValue> attributes, Delimiter delimiter);
            QMap<QString,AttributeValue> readAttributesFilePath(const QString& filePath, Delimiter delimiter);

        private:
            Delimiter m_attributeDelimiter{Delimiter::BLANK};
        };
    }//End of Archive
}//End of SSDK

#endif // TXT_HPP
