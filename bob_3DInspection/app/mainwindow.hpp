#ifndef JOBLOADER_HPP
#define JOBLOADER_HPP

#include <QDir>
#include <QString>

#include "../sdk/DB/sqlitedb.hpp"
#include "../job/inspectiondata.hpp"
#include "./datageneration.hpp"

using namespace std;
using namespace Job;
using namespace SSDK::DB;

namespace App
{
    //当目录下没有检测程式文件,则随机生成,此变量为随机生成检测对象的数量
    #define OBJ_CNT 50

    /**
     *  @brief MainWindow
     *         1.扫面目录下的检测程式,并将其显示在终端上
     *         2.读取检测程式文件的数据,将数据显示在终端上并将数据写入到xml文件中
     *         3.如果目录下没有检测程式,则自动生成一笔检测程式数据
     *  @author bob
     *  @version 1.00 2017-11-29 bob
     *                note:create it
     */
    class MainWindow
    {
    public:
        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //构造&析构函数
        MainWindow();

        ~MainWindow();
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //成员函数
        /*
        *  @brief  loadJob
        *          加载检测程式(文件)
        *          如果目录中没有检测程式文件,则随机生成一个检测程式,将随机生成数据
        *              写入到xml文件中,并将数据在终端上显示
        *          如果目录中有检测程式文件,则将检测程式文件显示在终端上,供用户选择
        *          根据选择的检测程式文件读取检测程式中的数据
        *          将读取的数据写入到xml文件中,并在终端上显示
        *  @param  N/A
        *  @return N/A
        */
        void loadJob(QString path);

        /*
        *  @brief   writeInspectionDataToJob
        *           将检测程式的信息写入至指定路径的检测程式文件中,具体信息如下
        *           检测程式的版本,最后依次编辑时间
        *           检测程式对应基板的名称,原点x ,y坐标,及长和宽
        *           基板中所有检查对象的名称,x,y轴坐标,及长和宽
        *           2017.12.02  添加写入检测对象的角度
        *  @param   path : 写入文件的路径 (如 :"./V1")
        *           pInspectionData: 需要写入到检测程式数据的地址
        *                            (即指向inspectionData的头地址)
        *  @return  N/A
        */
        void writeInspectionDataToJob(string path,
                                      InspectionData *pInspectionData);

        /*
        *  @brief  readInspectionDataFromJob
        *           从检测程式中读取数据,具体数据信息如下:
        *           检测程式的版本,最后依次编辑时间
        *           检测程式对应基板的名称,原点x ,y坐标,及长和宽
        *           基板中所有检查对象的名称,x,y轴坐标,及长和宽
        *           //2017.12.02 添加读取检测对象的角度
        *  @param   size: 文件中 measuredObj(检测对象)的数量
        *           pInspectionData : 指向存放检测程式数据的头指针
        *           measuredObjArr : 存放检测对象数据的数组
        *           sqlite : 检测程式文件的地址(即读取已打开的检测程式中的数据)
        *  @return  N/A
        */
        void readInspectionDataFromJob(int objCnt,
                                       InspectionData * pInspectionData,
                                       MeasuredObj measuredObjArr[],
                                       SqliteDB *sqlite);

        /*
        *  @brief  convertJobToV2
        *          将V1版本的检测程式转换成V2
        *          将检测程式的版本信息改写为V2
        *          将所有的检测对象添加angle,并设置为默认值0
        *  @param  sqlite:需要转换成V2版本的检测程式
        *  @return N/A
        */
        void convertJobToV2(SqliteDB * sqlite);
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    };
}  //End of namespace App

#endif // JOBLOADER_HPP
