#ifndef DATAGENERATION_HPP
#define DATAGENERATION_HPP

#include <time.h>

#include "../job/inspectiondata.hpp"
#include "../sdk/formatconvertion.hpp"
#include "../sdk/numrandom.hpp"

using namespace std;
using namespace Job;
using namespace SSDK;

namespace App
{
    /**
     *  @brief DataGeneration
     *         随机生成一笔检测程式的数据,具体数据如下:
     *         检测程式的版本,最后一次保存时间
     *         基板的名称,x,y轴原点坐标,基板的长和宽
     *         所有元件的名称,x,y轴坐标,角度,宽和高
     *  @author bob
     *  @version 1.00 2017-11-30 bob
     *                note:create it
     */
    class DataGeneration
    {
    public:
        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //构造 & 析构函数
        DataGeneration();

        ~DataGeneration();
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        /*
        *  @brief   generateInspectionData
        *           随机生成检测数据,具体信息如下:
        *           检测程式的版本
        *           检测程式对应基板的名称,原点x ,y坐标,及长和宽
        *           基板中所有元件的名称,x,y轴坐标,及长和宽
        *  @param   size:检测对象的数量
        *           inspectionData:存放inspectionData数据的头指针
        *           measuredObjArr:存放measuredObj对象的数组
        *  @return  N/A
        */
        void generateInspectionData(int size,
                                    InspectionData *pInspectionData,
                                    MeasuredObj measuredObjArr[]);
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    };
}   //End of namespace App

#endif // DATAGENERATION_HPP
