#include "datageneration.hpp"

using namespace std;
using namespace App;
using namespace Job;
using namespace SSDK;


DataGeneration::DataGeneration()
{

}

DataGeneration::~DataGeneration()
{

}

void DataGeneration::generateData(int size, MeasuredObj measuredObjArr[])
{
    srand( (unsigned)time( NULL ) );    //初始化随机数

    std::string namePrefix;             //元件名的前缀
    std::string nameIndex;              //元件名字的索引号
    std::string name;                   //元件名
    FormatConvertion formatConvertion;  //实例化一个对象,将元件索引号转换为字符串

    //根据传入参数,生成指定大小的链表
    for (int i = 0; i < size; ++i)
    {
        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //step 1
        //生成一个rectangle对象,x,y坐标,角度,及长和宽均为随机生成
        auto rect = Rectangle(RANDOM_NUM,
                              RANDOM_NUM,
                              RANDOM_NUM,
                              RANDOM_NUM,
                              RANDOM_NUM);
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //step 2
        //给measureObj数组中的元素赋值
        //生成前20笔数据为chip元件
        //生成20笔以后数据为ic元件
        if(i < 20)
        {
            namePrefix = "chip";
            nameIndex = formatConvertion.intToString(i + 1);
            name = namePrefix + nameIndex;
        }
        else
        {
            namePrefix = "ic";
            nameIndex = formatConvertion.intToString(i - 19);
            name = namePrefix + nameIndex;
        }
        measuredObjArr[i].setRectangle(&rect);      //将当前对象的rectangle变量赋值
        measuredObjArr[i].setName(name);            //设置已检测对象的名称
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    }
}
