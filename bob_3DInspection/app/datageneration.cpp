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

void DataGeneration::generateInspectionData(int size,
                                            InspectionData * pInspectionData,
                                            MeasuredObj measuredObjArr[])
{
    try
    {
        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //step1
        //随机生成指定数量的MeasureObj数据,并将数据添加至 measurdObjList 中
        srand( (unsigned)time( NULL ) );    //初始化随机数

        double max = 100;                   //定义生成随机浮点数最大值为100(不包括100)
        double min = 0;                     //定义生成浮点数的最小值为0(不包括0)

        NumRandom randomNum;

        //根据传入参数,生成指定大小的链表
        for (int i = 0; i < size; ++i)
        {
            //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //step1.1
            //生成一个rectangle对象,x,y坐标,角度,及长和宽均为随机生成
            //2017.12.02 bob 添加生成检测对象的角度
            auto rect = Rectangle(randomNum.randomDouble(max,min),
                                  randomNum.randomDouble(max,min),
                                  randomNum.randomDouble(max,min),
                                  randomNum.randomDouble(max,min),
                                  randomNum.randomDouble(max,min));
            //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

            //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //step1.2
            //给measureObj数组中的元素赋值
            //生成前20笔数据为chip元件
            //生成20笔以后数据为ic元件
            string namePrefix;                  //元件名的前缀
            string nameIndex;                   //元件名字的索引号
            string name;                        //元件名

            int chipCnt = 20;                   //生成chip元件的数量

            //随机生成20笔chip元件,30笔ic元件
            if(i < chipCnt)
            {
                namePrefix = "chip";                            //元件名的前缀
                nameIndex = FormatConvertion::intToString(i+1); //当前元件的索引号
                name = namePrefix + nameIndex;
            }
            else
            {
                namePrefix = "ic";
                nameIndex = FormatConvertion::intToString(i - chipCnt + 1);
                name = namePrefix + nameIndex;
            }
            measuredObjArr[i].setRectangle(&rect); //将当前对象的rectangle变量赋值
            measuredObjArr[i].setName(name);       //设置已检测对象的名称
            //将当前检测对象添加至链表中
            pInspectionData->pBoard()->pMeasuredObjList()->pushTail(&measuredObjArr[i]);
            //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        }
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //step2
        //生成board数据(基板名称,x,y原点坐标,基板的长和宽)
        //定义基板x,y原点坐标为10,长和宽均为100
        double originalX = 10, originalY = 10, sizeX = 100, sizeY = 100;
        pInspectionData->pBoard()->setName("iphone");
        pInspectionData->pBoard()->setOriginalX(originalX);
        pInspectionData->pBoard()->setOriginalY(originalY);
        pInspectionData->pBoard()->setSizeX(sizeX);
        pInspectionData->pBoard()->setSizeY(sizeY);
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //step3
        //生成inspectionData数据(检测程式的版本信息,检测程式上次保存时间)
        //2017.12.02 bob    将自动生成检测程式的版本号设置为V2
        string versionName = "V2";
        pInspectionData->setVersion(versionName);

        time_t currentTime = time(NULL);
        tm * pTime = localtime(&currentTime);       //获取当前系统时间的地址
        //获取时间,格式为 ( xx年/xx月/xx日  x时:x分:x秒)
        string lastEditingTime = FormatConvertion::intToString(pTime->tm_year+1900) +
              "/" + FormatConvertion::intToString(pTime->tm_mon+1) +
              "/" + FormatConvertion::intToString(pTime->tm_mday) +
              " " + FormatConvertion::intToString(pTime->tm_hour) +
              ":" + FormatConvertion::intToString(pTime->tm_min) +
              ":" + FormatConvertion::intToString(pTime->tm_sec);
        pInspectionData->setLastEditingTime(lastEditingTime);
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    }
    catch(const exception &ex)
    {
        THROW_EXCEPTION(ex.what());
    }
}
