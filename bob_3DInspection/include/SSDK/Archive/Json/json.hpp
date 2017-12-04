#ifndef JSONHELPER_H
#define JSONHELPER_H

#include <string>
#include <type_traits>
#include <map>
#include <iostream>
#include <fstream>
#include <memory>
#include <stdio.h>
#include <math.h>

#include <boost/variant/variant.hpp>
#include <boost/variant.hpp>

#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>
//#include <rapidjson/filestream.h>
#include <rapidjson/prettywriter.h>

#include "Exception/customexception.hpp"
#include "TypeTraits/typetraits.hpp"

    namespace SSDK
    {
        namespace Archive
        {
            /**
            *  @brief This class is used to wrap Json operation
            *   参考:
            *         1.refer to <<深入应用c++11>> P310 “rapidjson的基本用法”
            *         2.rapidjson首页说明：http://rapidjson.org/zh-cn/
            *
            *         rapidjson基本操作:
            *               1.读操作: 从文件，从字符串，从对象(反序列化)
            *               2.创建一个Json对象
            *               3.写文件: 到文件, 序列化
            *               4.查询Object, 查询Array
            *               5.修改成员的值
            *               6.删除/增加成员
            *               7.删除/增加/清空Array的成员
            *
            *  @author rime
            *  @version 1.00 2017-04-06 rime
            *                note:create it
            */
        class Json
        {
            public:

                //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                //enum & unsing

                using JsonWriter = rapidjson::PrettyWriter<rapidjson::StringBuffer>;

                /**
                 *RapidJson的数据类型分为以下几种：
                 * union Data {
                 *                String s;(可以为null)
                 *                Number n;(bool,int,uint,int64,uint64,double)
                 *                Object o;(复杂对象, 可以包含各种类型的Value,所以它是老大,可以为null)
                 *                Array a;
                 *            };
                 *
                 * 这4种类型都成为Value
                 * */

                /**
                 * @brief The ValueType enum
                 *              Value的类型
                 *               对于最上层Value的类型(也就是doc), 对于Json只可能是Object和Array
                 */
                enum ValueType
                {
                    NUMBER,
                    STRING,
                    ARRAY,
                    OBJECT
                };

                //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                //constructor & deconstructor

                /**
                 * 仅仅是创建一个对象，没有任何数据
                 */
                Json();

                /**
                 * @brief Json
                 *     创建Json对象后进行读取操作
                 * @param str
                 *      1.当从文件读取时，str表示文件路径
                 *      2.当从字符串读取时，str表示读取的字符串
                 * @param isFromString
                 *      true:从字符串读取
                 *      false:从文件读取
                 */
                Json(const std::string& str, bool isFromString);
                Json(const QString &str,bool isFromString);

                virtual ~Json();

                //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                //get & set functions

                 rapidjson::Value& doc(){return this->m_doc;}
                 JsonWriter& write(){return this->m_writer;}

                //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                //read functions

                /**
                 * @brief loadFromFile
                 *             读取一个Json文件
                 * @param filePath
                 *              文件路径
                 *              如果文件不存在或者打开失败，该函数不进行任何操作
                 */
                void loadFromFile(const std::string& filePath);
                void loadFromFile(const QString& filePath);

//                void loadFromDoc(const rapidjson::Document& doc);

                /**
                 * @brief parseFromString
                 *          从字符串解析得到一个Json对象
                 * @param JsonString
                 *          对应Json的字符串
                 */
                void parseFromString(const std::string& jsonString);
                void parseFromString(const QString& jsonString);
                void parseFromString(const char*  jsonStr);

                /**
                 * 返回对象序列化后端的json字符串
                 */
                const char* toString() ;

                /**
                 * @brief docType
                 * @return
                 *              获取Doc的类型, 只可能是Object或者Array
                 */
                ValueType docType();

                //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                //write functions

                //>>>-------------------------------------------------------------------------------------------------------------------------------------
                //1. write string for json creation

                /**
                 *writeValue 创建一个Json对象的时候通过writeValue写键值对
                 */

                template<typename V>//int
                typename std::enable_if<std::is_same<V, int>::value>::type
                writeValue(const V& value);

                template<typename V>//int
                typename std::enable_if<std::is_same<V, uint>::value>::type
                writeValue(const V& value);

                template<typename V>//long unsigned int(uint64)
                typename std::enable_if<std::is_same<V,uint64_t>::value>::type
                writeValue(const V& value);

                template<typename V>//int64
                typename std::enable_if<std::is_same<V, int64_t>::value>::type
                writeValue(const V& value);

                template<typename V>//float or double
                typename std::enable_if<std::is_floating_point<V>::value>::type
                writeValue(const V& value);

                template<typename V>//bool
                typename std::enable_if<std::is_same<V, bool>::value>::type
                writeValue(const V& value);

                void writeValue(const char* value);

                template<typename V>//null
                typename std::enable_if<std::is_same<V, std::nullptr_t>::value>::type
                writeValue(const V& value);

                template<typename T>
                void writeValue(const char* key, const T& value);

                /**
                 * 序列化结构体数组之前需要调用此接口，然后再循环去序列化
                 */
                void startArray();
                void endArray();

                /**
                 *每次写一个对象都要调用startObject函数，结束调用endObject
                 */
                void startObject();
                void endObject();

                //>>>-------------------------------------------------------------------------------------------------------------------------------------
                //2. serialization

                /**
                 * @brief writeToFile
                 *             写一个Json对象到文件
                 * @param filePath
                 *              文件保存路径
                 */
                void writeToFile(const QString& filePath);
                void writeToFile(const std::string& filePath);

                //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                //query  function

                //>>>-------------------------------------------------------------------------------------------------------------------------------------
                //0.Value

                /**
                 * @brief queryValue
                 *              在obj查找指定key的Value
                 * @param parentKey
                 *              父Value的Name
                 * @param childKey
                 *              子Value的Name
                 * @param isFound
                 *               是否找到对应的Value, 如果找到返回true,否则返回false
                 * @return
                 *              返回查找到的Value
                 */
                rapidjson::Value&  queryValue( const char* parentKey, const char* childKey, bool& isFound);
                /**
                   * @brief queryValue
                   *              在obj查找指定key的对象.
                   * @param key
                   *               查找的起始value
                   * @param isFound
                   *                输入参数:是否已经到对对象, 找到设置为true, 否则设置为false
                   * @return
                   *                返回查找到的Value
                   */
                rapidjson::Value& queryValue(const char* key, bool& isFound);
                /**
                  * @brief queryValue
                  *              在obj查找指定key的Value.
                  * @param obj
                  *              查找的起始value
                  * @param key
                  *              需要查找的对象的name
                  * @param isFound
                  *                输入参数:是否已经到对对象, 找到设置为true, 否则设置为false
                  * @return
                  *                输出参数:是否已经找到对象
                  *
                  * 注意:
                  *  1.该函数能够遍历json树，
                  *     如果找到对象key的对象，返回第一个满足条件的对象，返回true
                  *     如果没有任何满足条件的对象，返回false
                  *
                   *  2.该函数只支持在Object中查找, 无法搜索在Array中的对象, rapid中FindMember本身就不支持查找Array对象
                  */
                static rapidjson::Value&  queryValue( rapidjson::Value& obj, const char* key, bool& isFound);

                //>>>-------------------------------------------------------------------------------------------------------------------------------------
                //1.Number

                /**
                 * @brief queryNumber
                 *             查询指定key的Number, 包含了多个重载函数
                 * @param parentKey
                 *             Number所在的上一层Value的Name
                 * @param childKey
                 *             Number所在的Value的Name
                 * @return
                 *             返回查找到的Number
                 *
                 *  注意:
                 *          1.childKey任意一个为key都会抛出异常, parentKey为null时默认从最上层开始查找
                 *          2.没有key的成员或者key的类型和T的类型不匹配时，会抛出异常
                 *          3.当key的成员类型为数组时, 会被忽略掉, 最终还是会触发没有找到的异常
                 *          4.当key的成员存在并类型匹配时，返回该成员
                 *          5.使用enable_if进行函数的重载时，能够实现仅仅返回参数类型不同的重载（其它方式做不到）
                 *
                 */
                template<typename T>
                T queryNumber(const char* parentKey, const char* childKey);

                /**
                 *和上面的重载函数类似,只不过没有parentKey参数,那么默认就是从json的最上层开始检索
                 * 这样可能存在Json文件中不同的Object中含有相同Name的Value, 这种情况下类型不一致就会引发异常
                 */
                template<typename T>
                T queryNumber(const char* key);

                /**
                 * @brief queryNumber
                 *             查询指定key的Number
                 * @param value
                 *              查找Number所在的父Value
                 * @param key
                 *              Number所在的Value的Name
                 * @param isFound
                 *              是否已经找到, 找到返回true, 没找到的话返回false
                 * @return
                 *              查找到的Number, isFound为false, 请忽略该值
                 */
                template<typename T>
                static T queryNumber(rapidjson::Value& value,const char* key,bool& isFound);

                //>>>-------------------------------------------------------------------------------------------------------------------------------------
                //2.String

                /**
                * @brief queryString
                *             查询指定key的Number, 包含了多个重载函数
                * @param parentKey
                *             String所在的上一层Value的Name
                * @param childKey
                *             String所在的Value的Name
                * @return
                *             返回查找到的String
                *
                *  注意:
                *          1.childKey任意一个为key都会抛出异常, parentKey为null时默认从最上层开始查找
                *          2.没有key的成员或者key的类型和T的类型不匹配时，会抛出异常
                *          3.当key的成员类型为数组时, 会被忽略掉, 最终还是会触发没有找到的异常
                *          4.当key的成员存在并类型匹配时，返回该成员
                *
                */
                const char* queryString(const char* parentKey, const char* childKey);

                /**
               *和上面的重载函数类似,只不过没有parentKey参数,那么默认就是从json的最上层开始检索
               * 这样可能存在Json文件中不同的Object中含有相同Name的Value, 这种情况下类型不一致就会引发异常
               */
                const char* queryString(const char* key);

                static const char* queryString(rapidjson::Value& value,bool& isTypeMatched);
                static const char* queryString(rapidjson::Value& value, const char* key, bool& isFound);


                //>>>-------------------------------------------------------------------------------------------------------------------------------------
                //3.Array

                /**
                 * 查询指定key的Array, 包含了多个重载函数
                 *
                 * 注意:
                 *         1.对于数组来说
                 *                  返回Value, 因为Array中可能包含不同的类型, 由调用方进一步解析
                 *                  当Array的所有成员都属于同一类型的话,那么可以直接返回一个vector, 但是需要进行类型验证
                 *         2.支持嵌套在一个Object层级结构中的数组对象的查询, 即Array并不是在Object的最上层
                 *         3.rapidjson不支持数组嵌套数组的情况(包括最新的版本)
                 */

                /**
                  * @brief querryArray
                  *             查询指定key的Array
                  * @param parentObjKey
                  *             父Object的Name
                  * @param arrayName
                  *             Array的Name
                  * @param isFound
                  *             输入参数:是否已经到对对象, 找到设置为true, 否则设置为false
                  * @return
                  *             找到的Array类型的Value
                  */
                rapidjson::Value& queryArray(const char* parentObjKey, const char* arrayName, bool& isFound);

                /**
                  *和上面的重载函数类似,只不过没有parentObjKey参数,那么默认就是从json的最上层开始检索
                  * 这样可能存在Json文件中不同的Object中含有相同Name的Value, 这种情况下类型不一致就会引发异常
                  */
                rapidjson::Value& queryArray(const char* arrayName,  bool& isFound);

                /**
                 * @brief queryArray
                 *              查询指定key的Array
                 * @param parentObjKey
                  *             父Object的Name
                 * @param arrayName
                 *              需要查找的数组的arrayName
                 * @param vector
                 *              输出参数, 查找到的对象放置在这个vector容器中
                 * @return
                 *              查询成功返回true, 否则返回false
                 *
                 * 注意:
                 *          只有当Array的成员类型完全一样时才生效, 否则返回false
                 */
                template<typename T>
                bool queryArray(const char* parentObjKey,const char* arrayName, std::vector<T>& vector);

                /**
                 * @brief queryArray
                 *              查询指定key的Array
                 * @param arrayName
                 *              需要查找的数组的arrayName
                 * @param vector
                 *              输出参数, 查找到的对象放置在这个vector容器中
                 * @return
                 *              查询成功返回true, 否则返回false
                 *
                 * 注意:
                 *          只有当Array的成员类型完全一样时才生效, 否则返回false
                 */
                template<typename T>
                bool queryArray(const char* arrayName, std::vector<T>& vector);

                /**
                 * @brief queryDocArrayBasedOnIndex
                 *              当doc为array类型时, 根据返回对象的值
                 * @param index
                 *              Array的索引
                 * @param elementName
                 *              数组成员的名称
                 * @return
                 *              返回的值
                 *
                 * 注意:
                 *          1.只有当doc为Array类型时,才能调用该方法, 否则会抛出异常
                 *          2.doc为Array时, elememt一般为简单类型, 即Number和String
                 *          3.elementName不能为null, 否则会抛出异常
                 */
                template<typename T>
                       typename std::enable_if<
                            (std::is_same<T, bool>::value)||
                            (std::is_same<T, int>::value)||
                            (std::is_same<T, uint>::value)||
                            (std::is_same<T, int64_t>::value)||
                            (std::is_same<T,uint64_t>::value)||
                            (std::is_same<T, double>::value),T>::type
                 queryNumberOfDocArray( int index, const char* elementName);

                 const char* queryStringOfDocArray( int index, const char* elementName);

//                 template<typename T>
//                 T queryDocArrayBasedOnIndex( int objIndex , int elementIndex);

                /**
                 * @brief queryEmbededArrayElementBasedOnIndex
                 *              当doc不是array类型时, 即array作为doc的一个嵌入数组, 根据索引返回对象的值
                 * @param parentObjKey
                 *              array对象所在的父对象的Key
                 * @param arrayName
                 *              嵌入对象的Name
                 * @param index
                 *              访问对象的索引
                 * @return
                 *              返回的值
                 *
                 * 注意:
                 *          该函数只有在数组中所有成员的类型一致时才能时候, 否则将抛出异常
                 */
                template<typename T>
                T queryEmbededArrayBasedOnIndex(const char* parentObjKey,const char* arrayName, uint index);
                template<typename T>
                T queryEmbededArrayBasedOnIndex(const char* arrayName, uint index);

                template<typename T>
                T queryLastElementOfEmbededArray(const char* parentObjKey,const char* arrayName);
                template<typename T>
                T queryLastElementOfEmbededArray(const char* arrayName);

                template<typename T>
                T queryFirstElementOfEmbededArray(const char* parentObjKey,const char* arrayName);
                template<typename T>
                T queryFirstElementOfEmbededArray(const char* arrayName);

                /**
                 * @brief getArrayCount
                 *             获取到Array成员的数量
                 * @param parentObjKey
                 *              Array所在的父Value
                 * @param arrayName
                 *              Array的名称
                 * @return
                 *              Array成员的数量
                 *
                 *   当获取失败时, 返回-1
                 */
                int getArrayCount(const char* parentObjKey, const char* arrayName);
                /**
                 *和上面的重载函数类似,只不过没有parentObjKey参数,那么默认就是从json的最上层开始检索
                 * 这样可能存在Json文件中不同的Object中含有相同Name的Value, 这种情况下类型不一致就会引发异常
                 */
                int getArrayCount(const char* arrayName);

                /**
                 * @brief getArrayCount
                 *              当Doc为Array类型时,返回其长度
                 * @return
                 *              Doc为数组的长度, 当Doc不是数组时,返回-1
                 */
                int getArrayCount();

                //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


                //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                //modify  functions

                //>>>-------------------------------------------------------------------------------------------------------------------------------------
                //1.Number

                /**
                  * @brief modifyNumber
                  *              修正指定key的value, 包含如下行为
                  *              对于Number, 具体是bool, int, uint, int64, uint64, double
                  * @param parentKey
                  *              父对象的name
                  * @param childKey
                  *              子对象的name
                  * @param value
                  *              需要修正的值
                  *
                  *  * 注意:
                  *          1.没有key的成员或者key的类型和T的类型不匹配时，会抛出异常
                  *          2.当key的成员存在并类型匹配时，会修正该值
                  *          3.虽然下面几个函数都是模板函数, 可以支持任何类型的输入, 但是内部调用的modify模板函数都是有类型收窄的, 一旦都不支持的类型输入,编译将报错
                  *          4.不支持原来是Number类型的成员对象设置为指定类型( 官方文档没有类似的set函数 )
                  */
                template<typename T>
                void modifyNumber(const char* parentKey, const char* childKey,   const T& value);
                template<typename T>
                void modifyNumber(const char* key,   const T& value);//主要用于const char*/char* ,这2中类型不会调用上面的重载

                //>>>-------------------------------------------------------------------------------------------------------------------------------------
                //2.String

                /**
                 * @brief modifyString
                 *              修正指定key的value, 包含如下行为
                 *              对于String类型, 原来是null类型的对象可以设置为string
                 * @param parentKey
                 *              父Value的name
                 * @param childKey
                 *              需要修改的value的name
                 *
                 * @param value
                 *              需要查找的值
                 *
                 *  * 注意:
                 *          1.没有key的成员或者key的类型和T的类型不匹配时，会抛出异常
                 *          2.当key的成员存在并类型匹配时，会修正该值
                 *          3.虽然下面几个函数都是模板函数, 可以支持任何类型的输入, 但是内部调用的modify模板函数都是有类型收窄的, 一旦都不支持的类型输入,编译将报错
                 *          4.不支持原来是null类型的成员对象设置为指定类型( 官方文档没有类似的set函数 )
                 *          5.不支持直接修正一个Value类型的成员（即需要指定到基础类型，如Number和String的修改，大对象的修改需要分解到小对象, 官方文档没有类似的set函数）
                 */
                void modifyString(const char* parentKey, const char* childKey,   const char* value);

                void modifyString(const char* key,   const char* value);

                //>>>-------------------------------------------------------------------------------------------------------------------------------------
                //3.Array

                /**
                 *  修改指定key的Array, 包含如下行为
                 *          1. 增加成员
                 *          2. 删除成员
                 *          3. 清空Array
                 *          4. 修改成员
                 */

                /**
                 * @brief pushValBackToArray
                 *             在Array末尾插入Value成员
                 * @param parentKey
                 *              父对象的Key
                 * @param childkey
                 *              Array的Key
                 * @param newElementVal
                 *              待插入的Value
                 * @return
                 *              插入成功返回true, 插入失败返回false
                 *
                 * 注意:
                 *          1.当没有找到指定Key的Array,抛出异常
                 *          2.当插入的Val类型无法匹配Array中现有成员的类型,抛出异常
                 */
                bool pushValBackToArray(const char* parentKey, const char* childkey,  rapidjson::Value& newElementVal);
                bool pushValBackToArray(const char* key,  rapidjson::Value& newElementVal);

                /**
                 * @brief pushNumberBackToArray
                 *             在Array末尾插入一个Number成员
                 * @param parentKey
                 *              父对象的Key
                 * @param childkey
                 *              Array的Key
                 * @param newElementVal
                 *              待插入的Value
                 * @return
                 *              插入成功返回true, 插入失败返回false
                 *
                 * 注意:
                 *          1.当没有找到指定Key的Array,抛出异常
                 *          2.当插入的Val类型无法匹配Array中现有成员的类型,抛出异常
                 */
                template<typename T>
                typename std::enable_if<
                        (std::is_same<T, bool>::value)||
                        (std::is_same<T, int>::value)||
                        (std::is_same<T, uint>::value)||
                        (std::is_same<T, int64_t>::value)||
                        (std::is_same<T, double>::value),  bool>::type
                 pushNumberBackToArray(const char* parentKey,const char* childkey,    const T& newElementVal);

                template<typename T>
                typename std::enable_if<
                        (std::is_same<T, bool>::value)||
                        (std::is_same<T, int>::value)||
                        (std::is_same<T, uint>::value)||
                        (std::is_same<T, int64_t>::value)||
                        (std::is_same<T, double>::value),  bool>::type
                 pushNumberBackToArray(const char* key,    const T& newElementVal);

                /**
                 * @brief pushStringBackToArray
                 *             在Array末尾插入Value成员
                 * @param parentkey
                 *              父对象的Key
                 * @param childkey
                 *              Array的Key
                 * @param newElementVal
                 *              待插入的Value
                 * @return
                 *              插入成功返回true, 插入失败返回false
                 */
                bool pushStringBackToArray(const char *parentkey,const char *childkey, const char *newElementVal);
                bool pushStringBackToArray(const char* key,  const char* newElementVal);

                /**
                 * @brief popBackFromArray
                 *            移除Array末尾的成员
                 * @param parentkey
                 *            父对象的Key
                 * @param childKey
                 *            Array的Key
                 * @return
                 *            插入成功返回true, 插入失败返回false
                 */
                bool popBackFromArray(const char *parentkey,const char* childKey);
                bool popBackFromArray(const char* key);

                bool clearArray(const char *parentkey,const char *childKey);
                bool clearArray(const char* key);

                /**
                 * @brief modifyElementOfArrayBasedOnIndex
                 *             修改Array指定index的值
                 * @param parentKey
                 *             Array的父对象的Key
                 * @param childKey
                 *             Array的Key
                 * @param index
                 *             修改Array的Index
                 * @param value
                 *              待修改的值
                 *
                 * 注意
                 * 涉及到修改单个成员:
                 *           1.如果Array的成员是复杂类型, 即成员由不同类型的Object组成,请使用queryArray得到数组的Array, 然后再手动修改
                 *           2.如果Array的成员是简单类型, 请使用modifyArrayElementBasedOnIndex修改
                 */
                template<typename T>
                bool modifyNumberElementOfArrayBasedOnIndex(const char* parentKey,const char* childKey, uint index, const T& value);

                template<typename T>
                bool modifyNumberElementOfArrayBasedOnIndex(const char* key, uint index, const T& value);

                bool modifyStringElementOfArrayBasedOnIndex(const char* parentKey,const char* childKey, uint index, const char* value);
                bool modifyStringElementOfArrayBasedOnIndex(const char* key, uint index, const char* value);

                //>>>-------------------------------------------------------------------------------------------------------------------------------------
                //4.Object

                /**
                 *  添加删除成员操作,
                 *          1.这里的操作是指针对Object类型的Value, 可以添加和删除成员, 不包括基础类型(Number&String)和Array
                 *          2.不同于查询, add/remove 的成员可能是复杂的对象，如果再进一步拆解成基础类型会很不方便，同时官网API也只是支持到Value
                 */

                /**
                 * @brief addValueToObject
                 *             增加一个Value到Object
                 * @param parentKey
                 *              需要被添加的Object的value
                 * @param childKey
                 *              需要添加的子Value的name
                 * @param childValue
                 *              需要添加的Value
                 *
                 */
                  void addValueToObject(const char* parentKey, const  char* childKey, const char* memName,  rapidjson::Value& childValue);
                  void addValueToObject(const char *key,const char* memName,  rapidjson::Value &childValue);

                  /**
                   * @brief Json::addStringToObject
                   *              增加一个String到Object
                   * @param parentKey
                   *               Object所在的父Object的name
                   * @param childKey
                   *               Object的name
                   * @param memName
                   *                待添加的Object的name
                   * @param memVal
                   *                待添加的Object的value
                   */
                  void addStringToObject(const char *parentKey, const char *childKey, const char *memName, const char* memVal);
                  void addStringToObject(const char *key, const char *memName, const char* memVal);

                  /**
                   * @brief addNumberToObject
                   *             增加一个Number到Object
                   * @paramt T
                   *              待添加Number的类型,这里可以为bool,int,uint,int64,uint64和double
                   * @param parentKey
                   *                Object所在的父Object的name
                   * @param childKey
                   *                Object的name
                   * @param memName
                   *                待添加的Object的name
                   * @param memVal
                   *                待添加的Object的value
                   */
                  template<typename T>
                  typename std::enable_if<
                                  (std::is_same<T, bool>::value)||
                                  (std::is_same<T, int>::value)||
                                  (std::is_same<T, uint>::value)||
                                  (std::is_same<T, int64_t>::value)||
                                  (std::is_same<T,uint64_t>::value)||
                                  (std::is_same<T, double>::value)>::type
                  addNumberToObject(const char *parentKey, const char *childKey, const char *memName, const T& memVal);

                  template<typename T>
                  typename std::enable_if<
                                  (std::is_same<T, bool>::value)||
                                  (std::is_same<T, int>::value)||
                                  (std::is_same<T, uint>::value)||
                                  (std::is_same<T, int64_t>::value)||
                                  (std::is_same<T,uint64_t>::value)||
                                  (std::is_same<T, double>::value)>::type
                  addNumberToObject(const char *key, const char *memName,  const T& memVal);

                  /**
                   * @brief removeMemberFromObject
                   *              从Object中移除成员
                   * @param parentKey
                   *                Object所在的父Object的name
                   * @param childKey
                   *                Object的name
                   * @param memName
                   *                待添加的Object的name
                   */
                  void removeMemberFromObject(const char *parentKey, const char *childKey,const char *memName);
                  void removeMemberFromObject(const char *key,const char *memName);

            private:

                //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                //member variant

                rapidjson::StringBuffer m_buf;
                JsonWriter m_writer;
                rapidjson::Document m_doc;

                //是否需要重新accept, 默认不需要, 但是当发生修改时,需要重写accept
                bool m_isReAccept{false};

                //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                //query functions

                //>>>-------------------------------------------------------------------------------------------------------------------------------------
                //1.Number
                /**
                 *queryNumber 在Value查询不同类型的Number
                 */

                template<typename T>
                typename std::enable_if<std::is_same<T, bool>::value, T>::type//bool
                static queryNumber(rapidjson::Value& value,  bool& isTypeMatched);

                template<typename T>
                typename std::enable_if<std::is_same<T, int>::value, T>::type//int
                static queryNumber(rapidjson::Value& value, bool& isTypeMatched);

                template<typename T>
                typename std::enable_if<std::is_same<T, uint>::value, T>::type//uint
                static queryNumber(rapidjson::Value& value, bool& isTypeMatched);

                template<typename T>
                typename std::enable_if<std::is_same<T, int64_t>::value, T>::type//int64
                static queryNumber(rapidjson::Value& value, bool& isTypeMatched);

                template<typename T>
                typename std::enable_if<std::is_same<T, uint64_t>::value, T>::type//uint64
                static queryNumber(rapidjson::Value& value, bool& isTypeMatched);

                template<typename T>
                typename std::enable_if<std::is_same<T,double>::value, T>::type//double or float
                static queryNumber(rapidjson::Value& value, bool& isTypeMatched);

                //>>>-------------------------------------------------------------------------------------------------------------------------------------
                //2.string
                //...

                //>>>-------------------------------------------------------------------------------------------------------------------------------------
                //3.Array
                /**
                 * @brief queryArray
                 *              查询指定key的Array
                 * @param parentObj
                 *              查找数组的起始Object
                 * @param arrayName
                 *              需要查找的数组的name
                 * @param resVal
                 *              输出参数, 查找到的对象
                 * @return
                 *              是否已经找到对象, 找到的话返回true;否则返回false
                 *
                 */
                rapidjson::Value&  queryArray( rapidjson::Value& parentObj, const char* arrayName,  bool& isFound);

                //>>>-------------------------------------------------------------------------------------------------------------------------------------
                //4.Object
                //...

                //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


                //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                //modify functions

                //>>>-------------------------------------------------------------------------------------------------------------------------------------
                //1.Number

                /**
                *以下的重载modify函数都是用于按照类型modify对应的key值
                * 1.当输入的模板类型不支持或者模板类型与key值类型不符合时,返回false
                * 2.设置成功返回true
                */

                template<typename T>
                typename std::enable_if<std::is_same<T, bool>::value, bool>::type//bool
                modifyNumber( rapidjson::Value& val,  const T& value);

                template<typename T>
                typename std::enable_if<std::is_same<T, int>::value, bool>::type//int
                modifyNumber( rapidjson::Value& val, const T& value);

                template<typename T>
                typename std::enable_if<std::is_same<T, uint>::value, bool>::type//uint
                modifyNumber( rapidjson::Value& val, const T& value);

                template<typename T>
                typename std::enable_if<std::is_same<T, int64_t>::value, bool>::type//int64_t
                modifyNumber( rapidjson::Value& val, const T& value);

                template<typename T>
                typename std::enable_if<std::is_same<T, uint64_t>::value, bool>::type//uint64_t
                modifyNumber( rapidjson::Value& val, const T& value);

                template<typename T>
                typename std::enable_if<std::is_same<T, double>::value, bool>::type//double
                modifyNumber( rapidjson::Value& val, const T& value);

                //>>>-------------------------------------------------------------------------------------------------------------------------------------
                //2.String

                 bool modifyString( rapidjson::Value& val,  const char* value);

                //>>>-------------------------------------------------------------------------------------------------------------------------------------
                //3.Array

                /**
                    *以下几个重载函数:pushElementToVector
                    *         在Array所有成员类型全部一致,并且为简单类型(string, number, nullptr)的情况下封装到一个外部传入的vector中
                    */

                template<typename T>
                typename std::enable_if<std::is_same<T, bool>::value, bool>::type//bool
                pushElementOfArrayToVector(const rapidjson::Value::ConstValueIterator& arrayElementVal, std::vector<T>& vector)const;

                template<typename T>
                typename std::enable_if<std::is_same<T, int>::value, bool>::type//int
                pushElementOfArrayToVector(const rapidjson::Value::ConstValueIterator& arrayElementVal, std::vector<T>& vector)const;

                template<typename T>
                typename std::enable_if<std::is_same<T, int64_t>::value, bool>::type//int64_t
                pushElementOfArrayToVector(const rapidjson::Value::ConstValueIterator& arrayElementVal, std::vector<T>& vector)const;

                template<typename T>
                typename std::enable_if<std::is_same<T, uint64_t>::value, bool>::type//uint64_t
                pushElementOfArrayToVector(const rapidjson::Value::ConstValueIterator& arrayElementVal, std::vector<T>& vector)const;

                template<typename T>
                typename std::enable_if<std::is_same<T, double>::value, bool>::type//double
                pushElementOfArrayToVector(const rapidjson::Value::ConstValueIterator& arrayElementVal, std::vector<T>& vector)const;

                template<typename T>
                typename std::enable_if<std::is_same<T, std::string>::value, bool>::type//string
                pushElementOfArrayToVector(const rapidjson::Value::ConstValueIterator& arrayElementVal, std::vector<T>& vector)const;

                template<typename T>
                typename std::enable_if<std::is_same<char*, T>::value || std::is_same<const char*,T>::value, bool>::type//char* or const char*
                pushElementOfArrayToVector(const rapidjson::Value::ConstValueIterator& arrayElementVal, std::vector<T>& vector)const;

                //>>>-------------------------------------------------------------------------------------------------------------------------------------
                //4.Object
                 //...


                //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


        };//End of Json
        }//End of namespace Archive
    }//End of namespace SSDK

    //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //write functions

    template<typename V>//int
    typename std::enable_if<std::is_same<V, int>::value>::type
    SSDK::Archive::Json::writeValue(const V& value)
    {
        m_writer.Int(value);
    }

    template<typename V>//int
    typename std::enable_if<std::is_same<V, uint>::value>::type
    SSDK::Archive::Json::writeValue(const V& value)
    {
        m_writer.Uint(value);
    }

    template<typename V>//long unsigned int(uint64)
    typename std::enable_if<std::is_same<V,uint64_t>::value>::type
    SSDK::Archive::Json::writeValue(const V& value)
    {
        m_writer.Uint64(value);
    }

    template<typename V>//int64
    typename std::enable_if<std::is_same<V, int64_t>::value>::type
    SSDK::Archive::Json::writeValue(const V& value)
    {
        m_writer.Int64(value);
    }

    template<typename V>//float or double
    typename std::enable_if<std::is_floating_point<V>::value>::type
    SSDK::Archive::Json::writeValue(const V& value)
    {
        m_writer.Double(value);
    }

    template<typename V>//bool
    typename std::enable_if<std::is_same<V, bool>::value>::type
    SSDK::Archive::Json::writeValue(const V& value)
    {
        m_writer.Bool(value);
    }

    template<typename V>//null
    typename std::enable_if<std::is_same<V, std::nullptr_t>::value>::type
    SSDK::Archive::Json::writeValue(const V& value)
    {
        m_writer.Null();
    }

    template<typename T>
    void  SSDK::Archive::Json::writeValue(const char* key, const T& value)
    {
        m_writer.String(key);
        writeValue(value);
    }

    //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //query functions

    //>>>-------------------------------------------------------------------------------------------------------------------------------------
    //1.Number

    template<typename T>
    T SSDK::Archive::Json::queryNumber(const char* key)
   {
       return queryNumber<T>(nullptr, key);
   }

    template<typename T>
   T SSDK::Archive::Json::queryNumber(const char* parentKey, const char* childKey)
   {
           using namespace rapidjson;

           bool isFound = false;
           auto& childVal = queryValue(parentKey,childKey,isFound);//找出子Value
           if(isFound)
           {
               bool isTypeMatched = false;
               auto val = queryNumber<T>(childVal,isTypeMatched);
               if(!isTypeMatched)
               {
                    THROW_EXCEPTION_WITH_OBJ("Type of Member["+std::string(childKey)+"] is not matched!");
               }
               else
               {
                   return val;
               }
           }
           else
           {
                  THROW_EXCEPTION_WITH_OBJ("Member["+std::string(childKey)+"] is not found!");
           }
   }

    template<typename T>
    typename std::enable_if<std::is_same<T, bool>::value, T>::type//bool
    SSDK::Archive::Json::queryNumber(rapidjson::Value& value,  bool& isTypeMatched)
    {
        isTypeMatched = false;
        if(value.IsBool())
        {
            isTypeMatched = true;
            return value.GetBool();
        }
        else
        {
            return false;
        }
    }

   template<typename T>
    T SSDK::Archive::Json::queryNumber(rapidjson::Value& value,const char* key,bool& isFound)
  {
       isFound = false;
       auto& val = queryValue(value,key, isFound);
       return queryNumber<T>(val,isFound);
  }

    template<typename T>
    typename std::enable_if<std::is_same<T, int>::value, T>::type//int
    SSDK::Archive::Json::queryNumber(rapidjson::Value& value, bool& isTypeMatched)
    {
       isTypeMatched = false;
       if(value.IsInt())
       {
           isTypeMatched = true;
           return value.GetInt();
       }
       else
       {
           return 0;
       }
    }

    template<typename T>
    typename std::enable_if<std::is_same<T, uint>::value, T>::type//uint
    SSDK::Archive::Json::queryNumber(rapidjson::Value& value, bool& isTypeMatched)
    {
       isTypeMatched = false;
       if(value.IsUint())
       {
           isTypeMatched = true;
           return value.GetUint();
       }
       else
       {
           return 0;
       }
    }

    template<typename T>
    typename std::enable_if<std::is_same<T, int64_t>::value, T>::type//int64
    SSDK::Archive::Json::queryNumber(rapidjson::Value& value, bool& isTypeMatched)
    {
       isTypeMatched = false;
       if(value.IsInt64())
       {
           isTypeMatched = true;
           return value.GetInt64();
       }
       else
       {
           return 0;
       }
    }

    template<typename T>
    typename std::enable_if<std::is_same<T, uint64_t>::value, T>::type//uint64
    SSDK::Archive::Json::queryNumber(rapidjson::Value& value, bool& isTypeMatched)
    {
       isTypeMatched = false;
       if(value.IsUint64())
       {
           isTypeMatched = true;
           return value.GetUint64();
       }
       else
       {
           return 0;
       }
    }

    template<typename T>
    typename std::enable_if<std::is_same<T,double>::value, T>::type//double or float
    SSDK::Archive::Json::queryNumber(rapidjson::Value& value, bool& isTypeMatched)
    {
       isTypeMatched = false;
       if(value.IsDouble())
       {
           isTypeMatched = true;
           return value.GetDouble();
       }
       else
       {
           return 0.0;
       }
    }

   //>>>-------------------------------------------------------------------------------------------------------------------------------------
   //2.String

   //>>>-------------------------------------------------------------------------------------------------------------------------------------
   //3.Array

   template<typename T>
   bool SSDK::Archive::Json::queryArray(const char* arrayName, std::vector<T>& vector)
   {
       using namespace rapidjson;

       bool isFound = false;
       auto& val = queryArray(this->m_doc, arrayName, isFound);;
       if(isFound && val.IsArray())
       {
           //注意,这里要使用Begin(), 而不是MemberBegin
           for (auto  itr = val.Begin(); itr != val.End(); ++itr)
           {
               pushElementOfArrayToVector(itr,vector);
           }

           return true;
       }
       else
       {
           return false;
       }
   }
   template<typename T>
   bool SSDK::Archive::Json::queryArray(const char* parentObjKey,const char* arrayName, std::vector<T>& vector)
   {
       using namespace rapidjson;

       bool isFound = false;
       auto& val = queryValue(parentObjKey, arrayName, isFound);;
       if(isFound && val.IsArray())
       {
           //注意,这里要使用Begin(), 而不是MemberBegin
           for (auto  itr = val.Begin(); itr != val.End(); ++itr)
           {
               pushElementOfArrayToVector(itr,vector);
           }

           return true;
       }
       else
       {
           return false;
       }
   }

   template<typename T>
          typename std::enable_if<
               (std::is_same<T, bool>::value)||
               (std::is_same<T, int>::value)||
               (std::is_same<T, uint>::value)||
               (std::is_same<T, int64_t>::value)||
               (std::is_same<T,uint64_t>::value)||
               (std::is_same<T, double>::value),T>::type
    SSDK::Archive::Json::queryNumberOfDocArray( int index, const char* elementName)
    {
        if(nullptr == elementName)
        {
                THROW_EXCEPTION_WITH_OBJ("Element Name can not be nullptr");
        }

        if(this->m_doc.IsArray())
        {
            int elementCnt = this->m_doc.Size();
            if(index<0 || index>elementCnt-1)
            {
                std::ostringstream stream;
                stream<<"Index["<<index<<"] is invaild, it must be in[0,"<<elementCnt - 1<<"]";
                THROW_EXCEPTION_WITH_OBJ( stream.str());
            }
            else
            {
                bool isFound {false};
                auto& value = queryValue( this->m_doc[index],elementName,isFound);

                if(!isFound)
                {
                     THROW_EXCEPTION_WITH_OBJ("Array[Name:"+std::string(elementName)+"] is not found!" );
                }
                else
                {
                    bool isMatched {false};
                    return queryNumber<T>(value,isMatched);
                }
            }
        }
        else
        {
            THROW_EXCEPTION_WITH_OBJ("Doc must be array type");
        }
    }

   template<typename T>
   T SSDK::Archive::Json::queryEmbededArrayBasedOnIndex(const char* parentObjKey,const char* arrayName, uint index)
   {
       using namespace rapidjson;

       std::vector<T> vector;
       bool isFound = queryArray(parentObjKey,arrayName,vector);

       if(!isFound)
       {
              THROW_EXCEPTION_WITH_OBJ("Array[Name:"+std::string(arrayName)+"] is not found!" );
       }
       else
       {
           if(index > vector.size())
           {
               std::ostringstream stream;
               stream<<"Index["<<index<<"] is invaild, it must be in[0,"<<vector.size()<<"]";
               THROW_EXCEPTION_WITH_OBJ( stream.str() );
           }
           else
           {
               return vector[index];
           }
       }

       return true;
   }
   template<typename T>
   T SSDK::Archive::Json::queryEmbededArrayBasedOnIndex(const char* arrayName, uint index)
   {
       return queryEmbededArrayBasedOnIndex<T>(nullptr,arrayName,index);
   }

   template<typename T>
   T SSDK::Archive::Json::queryLastElementOfEmbededArray(const char* parentObjKey,const char* arrayName)
   {
       using namespace rapidjson;

       std::vector<T> vector;
       bool isFound = queryArray(parentObjKey,arrayName,vector);

       if(!isFound)
       {
           THROW_EXCEPTION_WITH_OBJ("Array[Name:"+std::string(arrayName)+"] is not found!" );
       }
       else
       {
              if(vector.size()==0)
              {
                  return vector[0];
              }
              else
              {
                 return vector[vector.size()-1];
              }
       }
   }
   template<typename T>
   T SSDK::Archive::Json::queryLastElementOfEmbededArray(const char* arrayName)
   {
       return queryLastElementOfEmbededArray<T>(nullptr,arrayName);
   }

   template<typename T>
   T SSDK::Archive::Json::queryFirstElementOfEmbededArray(const char* parentObjKey,const char* arrayName)
   {
       using namespace rapidjson;

       std::vector<T> vector;
       bool isFound = queryArray(parentObjKey,arrayName,vector);

       if(!isFound)
       {
           THROW_EXCEPTION_WITH_OBJ("Array[Name:"+std::string(arrayName)+"] is not found!" );
       }
       else
       {
           if(!isFound)
           {
               THROW_EXCEPTION_WITH_OBJ("Array[Name:"+std::string(arrayName)+"] is not found!" );
           }
           else
           {
                   return vector[0];
           }
       }

//       return true;
   }
   template<typename T>
   T  SSDK::Archive::Json::queryFirstElementOfEmbededArray(const char* arrayName)
   {
       return queryFirstElementOfEmbededArray<T>(nullptr, arrayName);
   }

   //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

   //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
   //modify functions

   //>>>-------------------------------------------------------------------------------------------------------------------------------------
   //1.Number

   template<typename T>
   void SSDK::Archive::Json::modifyNumber(const char* parentKey, const char* childKey,   const T& value)
   {
       using namespace rapidjson;
       bool isFound{false};
       auto& val = queryValue(parentKey,childKey,isFound);
       if(isFound)//对于rapidjson v1.0 需要判断是否为0, 对于最新的v1.1 要判断是否为EndMember
       {
           if(!modifyNumber<T>(val, value))
           {
               THROW_EXCEPTION_WITH_OBJ("Member["+std::string(parentKey)+"] is bool type!");
           }

           this->m_isReAccept = true;
       }
       else
       {
           THROW_EXCEPTION_WITH_OBJ("Member["+std::string(parentKey)+"] is not found!");
       }
   }
   template<typename T>
   void SSDK::Archive::Json::modifyNumber(const char* key,   const T& value)//主要用于const char*/char* ,这2中类型不会调用上面的重载
   {
       modifyNumber(nullptr,key,value);
   }


   template<typename T>
   typename std::enable_if<std::is_same<T, bool>::value, bool>::type//bool
   SSDK::Archive::Json::modifyNumber( rapidjson::Value& val,  const T& value)
   {
       if(val.IsBool())
       {
           val.SetBool(value);
           return true;
       }
       else
       {
           return false;
       }
   }

   template<typename T>
   typename std::enable_if<std::is_same<T, int>::value, bool>::type//int
   SSDK::Archive::Json::modifyNumber( rapidjson::Value& val, const T& value)
   {
       if(val.IsInt())
       {
           val.SetInt(value);
           return true;
       }
       else
       {
           return false;
       }
   }

   template<typename T>
   typename std::enable_if<std::is_same<T, uint>::value, bool>::type//uint
   SSDK::Archive::Json::modifyNumber( rapidjson::Value& val, const T& value)
   {
       if(val.IsUint())
       {
           val.SetUint(value);
           return true;
       }
       else
       {
           return false;
       }
   }

   template<typename T>
   typename std::enable_if<std::is_same<T, int64_t>::value, bool>::type//int64_t
   SSDK::Archive::Json::modifyNumber( rapidjson::Value& val, const T& value)
   {
       if(val.IsInt64())
       {
           val.SetInt64(value);
           return true;
       }
       else
       {
           return false;
       }
   }

   template<typename T>
   typename std::enable_if<std::is_same<T, uint64_t>::value, bool>::type//uint64_t
   SSDK::Archive::Json::modifyNumber( rapidjson::Value& val, const T& value)
   {
       if(val.IsUint64())
       {
           val.SetUint64(value);
           return true;
       }
       else
       {
           return false;
       }
   }

   template<typename T>
   typename std::enable_if<std::is_same<T, double>::value, bool>::type//double
   SSDK::Archive::Json::modifyNumber( rapidjson::Value& val, const T& value)
   {
       if(val.IsDouble())
       {
           val.SetDouble(value);
           return true;
       }
       else
       {
           return false;
       }
   }

   //>>>-------------------------------------------------------------------------------------------------------------------------------------
   //3.Array

   /**
    * @brief modifyElementOfArrayBasedOnIndex
    *             修改Array指定index的值
    * @param parentKey
    *             Array的父对象的Key
    * @param childKey
    *             Array的Key
    * @param index
    *             修改Array的Index
    * @param value
    *              待修改的值
    *
    * 注意
    * 涉及到修改单个成员:
    *           1.如果Array的成员是复杂类型, 即成员由不同类型的Object组成,请使用queryArray得到数组的Array, 然后再手动修改
    *           2.如果Array的成员是简单类型, 请使用modifyArrayElementBasedOnIndex修改
    */
   template<typename T>
   bool  SSDK::Archive::Json::modifyNumberElementOfArrayBasedOnIndex(const char* parentKey,const char* childKey, uint index, const T& value)
   {
       using namespace rapidjson;

       bool isFound = false;
       auto& val = queryValue(parentKey,childKey,isFound);
       if(isFound && val.IsArray())
       {
              if(index<0 || index > val.Size())
              {
                  std::ostringstream stream;
                  stream<<"Index["<<index<<"] is invaild, it must be in[0,"<<val.Size()<<"]";
                  THROW_EXCEPTION_WITH_OBJ( stream.str() );
              }
              else
              {
                  modifyNumber(val[index], value);
              }
       }
       else
       {
            THROW_EXCEPTION_WITH_OBJ("Member["+std::string(childKey)+"] is not found!");
       }
   }

   template<typename T>
   bool  SSDK::Archive::Json::modifyNumberElementOfArrayBasedOnIndex(const char* key, uint index, const T& value)
   {
       using namespace rapidjson;

       bool isSuccessful{false};
       bool isFound {false};
       auto& val = queryValue(this->m_doc,key,isFound);
       if(isFound && val.IsArray())
       {
              if( index > val.Size())
              {
                  std::ostringstream stream;
                  stream<<"Index["<<index<<"] is invaild, it must be in[0,"<<val.Size()<<"]";
                  THROW_EXCEPTION_WITH_OBJ( stream.str() );
              }
              else
              {
                  isSuccessful = modifyNumber(val[index], value);
              }
       }
       else
       {
            THROW_EXCEPTION_WITH_OBJ("Member["+std::string(key)+"] is not found!");
       }

       return isSuccessful;
   }

   template<typename T>
   typename std::enable_if<
   (std::is_same<T, bool>::value)||
   (std::is_same<T, int>::value)||
   (std::is_same<T, uint>::value)||
   (std::is_same<T, int64_t>::value)||
   (std::is_same<T, double>::value),  bool>::type
   SSDK::Archive::Json::pushNumberBackToArray(const char* parentKey,const char* childkey,    const T& newElementVal)
   {
       rapidjson::Value val(newElementVal);
       return pushValBackToArray(parentKey,childkey,val);
   }

   template<typename T>
           typename std::enable_if<
           (std::is_same<T, bool>::value)||
           (std::is_same<T, int>::value)||
           (std::is_same<T, uint>::value)||
           (std::is_same<T, int64_t>::value)||
           (std::is_same<T, double>::value),  bool>::type
   SSDK::Archive::Json::pushNumberBackToArray(const char* key,    const T& newElementVal)
   {
       return pushNumberBackToArray(nullptr,key,newElementVal);
   }

   template<typename T>
   typename std::enable_if<std::is_same<T, bool>::value, bool>::type//bool
   SSDK::Archive::Json::pushElementOfArrayToVector(const rapidjson::Value::ConstValueIterator& arrayElementVal, std::vector<T>& vector)const
   {
       if(arrayElementVal->IsBool())
       {
           vector.push_back(arrayElementVal->GetBool());
           return true;
       }
       else
       {
           return false;
       }
   }

   template<typename T>
   typename std::enable_if<std::is_same<T, int>::value, bool>::type//int
   SSDK::Archive::Json::pushElementOfArrayToVector(const rapidjson::Value::ConstValueIterator& arrayElementVal, std::vector<T>& vector)const
   {
       if(arrayElementVal->IsInt())
       {
           vector.push_back(arrayElementVal->GetInt());
           return true;
       }
       else
       {
           return false;
       }
   }

   template<typename T>
   typename std::enable_if<std::is_same<T, int64_t>::value, bool>::type//int64_t
   SSDK::Archive::Json::pushElementOfArrayToVector(const rapidjson::Value::ConstValueIterator& arrayElementVal, std::vector<T>& vector)const
   {
       if(arrayElementVal->IsInt64())
       {
           vector.push_back(arrayElementVal->GetInt64());
           return true;
       }
       else
       {
           return false;
       }
   }

   template<typename T>
   typename std::enable_if<std::is_same<T, uint64_t>::value, bool>::type//uint64_t
   SSDK::Archive::Json::pushElementOfArrayToVector(const rapidjson::Value::ConstValueIterator& arrayElementVal, std::vector<T>& vector)const
   {
       if(arrayElementVal->IsUint64())
       {
           vector.push_back(arrayElementVal->GetUint64());
           return true;
       }
       else
       {
           return false;
       }
   }

   template<typename T>
   typename std::enable_if<std::is_same<T, double>::value, bool>::type//double
   SSDK::Archive::Json::pushElementOfArrayToVector(const rapidjson::Value::ConstValueIterator& arrayElementVal, std::vector<T>& vector)const
   {
       if(arrayElementVal->IsDouble())
       {
           vector.push_back(arrayElementVal->GetDouble());
           return true;
       }
       else
       {
           return false;
       }
   }

   template<typename T>
   typename std::enable_if<std::is_same<T, std::string>::value, bool>::type//string
   SSDK::Archive::Json::pushElementOfArrayToVector(const rapidjson::Value::ConstValueIterator& arrayElementVal, std::vector<T>& vector)const
   {
       if(arrayElementVal->IsString())
       {
           vector.emplace_back(std::string(arrayElementVal->GetString()));
           return true;
       }
       else
       {
           return false;
       }
   }

   template<typename T>
   typename std::enable_if<std::is_same<char*, T>::value || std::is_same<const char*,T>::value, bool>::type//char* or const char*
   SSDK::Archive::Json::pushElementOfArrayToVector(const rapidjson::Value::ConstValueIterator& arrayElementVal, std::vector<T>& vector)const
   {
       if(arrayElementVal->IsString())
       {
           vector.push_back((T)arrayElementVal->GetString());
           return true;
       }
       else
       {
           return false;
       }
   }

   //>>>-------------------------------------------------------------------------------------------------------------------------------------
   //4.Object

   /**
    * @brief addNumberToObject
    *             增加一个Number到Object
    * @paramt T
    *              待添加Number的类型,这里可以为bool,int,uint,int64,uint64和double
    * @param parentKey
    *                Object所在的父Object的name
    * @param childKey
    *                Object的name
    * @param memName
    *                待添加的Object的name
    * @param memVal
    *                待添加的Object的value
    */
   template<typename T>
   typename std::enable_if<
       (std::is_same<T, bool>::value)||
       (std::is_same<T, int>::value)||
       (std::is_same<T, uint>::value)||
       (std::is_same<T, int64_t>::value)||
       (std::is_same<T,uint64_t>::value)||
       (std::is_same<T, double>::value)>::type
   SSDK::Archive::Json::addNumberToObject(const char *parentKey, const char *childKey, const char *memName, const T& memVal)
   {
       bool isFound{false};
       auto& parentObj = queryValue(parentKey, childKey,isFound);
       if(!isFound  && !parentObj.IsObject())
       {
           THROW_EXCEPTION_WITH_OBJ("Object["+std::string(childKey)+"] is not found!");
       }
       else
       {
           parentObj.AddMember(rapidjson::Value::StringRefType(memName),memVal,this->m_doc.GetAllocator());
           this->m_isReAccept = true;
       }
   }

   template<typename T>
   typename std::enable_if<
           (std::is_same<T, bool>::value)||
           (std::is_same<T, int>::value)||
           (std::is_same<T, uint>::value)||
           (std::is_same<T, int64_t>::value)||
           (std::is_same<T,uint64_t>::value)||
           (std::is_same<T, double>::value)>::type
   SSDK::Archive::Json::addNumberToObject(const char *key, const char *memName,  const T& memVal)
   {
       addNumberToObject(nullptr,key,memName,memVal);
   }



   //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------





#endif // JSONHELPER_H
