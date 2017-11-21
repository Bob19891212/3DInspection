#ifndef CAPTURESETTING_HPP
#define CAPTURESETTING_HPP


#include <iostream>

#include <QSettings>
#include <QFile>

#include "../sdk/customexception.hpp"

using namespace std;

namespace App
{
    /**
     *  @brief  CaptureSetting
     *          读取目录下的配置文件,如果目录下没有配置文件,则抛出异常
     *          如果目录下文件数据信息不正确,则抛出异常
     *  @author bob
     *  @version 1.00 2017-11-21 bob
     *                note:create it
     */
    class CaptureSetting
    {
    public:
        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //读写目录下的文件
        /*
        *  @brief  readCaptureSetting
        *          读取指定目录下的文件,判断文件目录是否存在,如果不存在则抛出异常
        *          如果文件数据不正确,则抛出异常
        *  @param  path:读取文件的路径,如(./CaptureSetting.ini)
        *  @return  N/A
        */
        void readCaptureSetting(const QString& path);

        /*
        *  @brief  writeCaptureSetting
        *          将根据数据格式将数据写入指定目录下的文件
        *  @param  path:写入数据文件的路径,如(./CaptureSetting.ini)
        *  @return  N/A
        */
        void writeCaptureSetting(const QString& path);

        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    private:
        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //定义图片的宽度,高度及图片格式的位数
        //读取文件中信息与默认值对比
        int imgWidth = 4096;                //定义图片的宽度为4096
        int imgHeight = 3072;               //定义图片的高度为3072
        int imgBitArr[2] = {8,16};          //定义图片位数格式为8位或16为

        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    };
} //End of namespace CaptureSetting

#endif // CAPTURESETTING_HPP