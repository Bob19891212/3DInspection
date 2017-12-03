#ifndef IJSONSERIALIZABLE_H
#define IJSONSERIALIZABLE_H

#include "string"

#include "json.hpp"

namespace SSDK
{
    namespace Archive
    {
    /**
     *  @brief IJsonSerializable
     *              需要支持Json按照默认的初始化方式的需要继承该类
     *
     *  @author rime
     *  @version 1.00 2017-05-08 rime
     *                note:create it
     *
     * 注意:
     *         1.这里的函数用于继承了IJsonSerializable的类实现默认的Json序列化和反序列化, 具体的逻辑全部在对应的类型中实现
     *         2.如果想要实现一些定制灵活的序列化/反序列化(如SPC或者指定报表,当然这种场景Json不是首选), 需要另外写工作类
     *            实现相应的序列化/反序列化代码
     *         3.序列化/反序列化需要考虑以下情况:
     *                  1.对象直接的序列化,即序列化到一个Json文件
     *                  2.序列化到一个Json对象, 作为一个更大对象的序列化流程中的一部分
     */
    class IJsonSerializable
    {
        public:
            IJsonSerializable(){}
            virtual ~IJsonSerializable(){}

            /**
             * @brief serializeToJsonObj
             *              序列化对象到Json对象中
             *
             * @param json
             *              输入参数:序列化到Json对象
             *
             * 注意:
             *     1.当B对象作为一个更大对象A的嵌入对象时,A的序列化过程中要实现B的序列化,如
             *                      A包含B, A序列化的过程中包好了B的序列化
             *              此时的做法是把A对象的Json对象传入, 在B中实现了序列化工作在这个Json对象中实现
             *      2.如果json为该类型本身的Json对象, 即json传入的就是B的json对象, 那么说明整个序列化只有B对象
             *      3.具体的类型不同,实现序列化的代码逻辑也不一样,所以这个函数一定是抽象虚函数
             */
            virtual void serializeToJsonObj(Json& json) = 0;

            /**
             * @brief serializeToJsonFile
             *             序列化对象到Json文件中
             * @param filePath
             *             Json文件的路径,需要保证路径合法
             *
             * 序列化一个对象,那么这个对象一个是Object类型的, 所以序列化流程一定是:
             *      1.startObject
             *      2.序列化对应的成员(serializeToJsonObj)
             *      3.endObject
             *      4.序列化到对应的路径(writeToFile)
             *
             * 该虚函数可以不是抽象函数
             */
            virtual void serializeToJsonFile(const std::string& filePath)
            {
                //TBC 加上路径验证

                this->m_json.startObject();
                serializeToJsonObj( this->m_json);
                this->m_json.endObject();
                this->m_json.writeToFile(filePath);
            }

            /**
              * @brief deserializeFromJsonValue
              *             从Value中反序列化得到对象的成员值
              * @param value
              *             输入参数:反序列化Value的值
              *
              * 注意:
              *     1.当B对象作为一个更大对象A的嵌入对象时, B的反序列化过程中要实现A的反序列化,如
              *                      A包含B, A反序列化的过程中包涵了B的反序列化
              *              此时的做法是把B对象的Value传入, 在B中实现了反序列化工
              *
              *      2.如果value为该类型本身的Json对象, 即value传入的就是B的json对象, 那么说明整个反序列化只有B对象
              *      3.不同于序列化, 反序列化需要考虑Value, 因为通过doc驱使, 一次性读入,不同层级可能发生重名,如果不指定Value
              *         会导致重名的Value而引发类型冲突
              *      4.具体的类型不同,实现反序列化的代码逻辑也不一样,所以这个函数一定是抽象虚函数
              */
             virtual void deserializeFromJsonValue(rapidjson::Value& value) = 0;

            /**
             * @brief deserializeFromFile
             *             从指定的Json文件反序列化
             * @param filePath
             *             Json文件的路径,需要保证路径合法
             *
             * 反序列化一个对象,那么这个对象一个是Object类型的, 所以流程一定是:
             *      1.loadFromFile
             *      2.从对应的Value序列化各个成员(deserializeFromJsonValue)
             *
             * 该虚函数可以不是抽象函数
             */
            virtual void deserializeFromFile(const std::string& filePath)
            {
                //TBC 加上路径验证

                this->m_json.loadFromFile(filePath);
                deserializeFromJsonValue(this->m_json.doc());
            }

        protected:
            Json m_json;
        };//End of IJsonSerializable
    }//End of namespace Archive
}//End of namespace SSDK

#endif // IJSONSERIALIZABLE_H
