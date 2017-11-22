#ifndef APPSETTING_HPP
#define APPSETTING_HPP

#include <iostream>

#include <QSettings>
#include <QFile>

#include "../sdk/customexception.hpp"

//宏函数,取出枚举中元素的字符串
#define VAR_TO_STR(var)\
    ({\
        std::string name = (#var);\
        int pos = 0;\
        int length = 0;\
        length = name.length();\
        pos = name.find_last_of(':',length);\
        if ( pos == -1 )\
        {\
            pos = 0;\
            name = name.substr(pos,length);\
        }\
        else\
        {\
            name = name.substr(length - pos,length);\
        }\
        (name);\
    })


namespace App
{
    /**
     *  @brief AppSetting
     *         读取目录下的配置文件,如果目录下没有配置文件,则自动生成默认值
     *         如果目录下文件数据信息不正确,则将文件数据写如默认值
     *  @author bob
     *  @version 1.00 2017-11-21 bob
     *                note:create it
     */
    class AppSetting
    {
    public:
        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //读写配置文件中的数据
        /*
        *  @brief  readAppSetting
        *          读取指定目录下的文件,判断文件目录是否存在,则写入默认配置文件
        *          如果文件数据不正确,则将数据写入默认值
        *  @param  path:读取文件的路径,如(./app.ini)
        *  @return  N/A
        */
        void readAppSetting(const QString& path);

        /*
        *  @brief  writeAppSetting
        *          将根据数据格式将数据写入指定路径下的文件
        *  @param  path:写入数据文件的路径,如(./app.ini)
        *  @return  N/A
        */
        void writeAppSetting(const QString& path);
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    private:
        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //成员变量,分别对应读取文件中的数据
        //声明软件的主题为枚举类型,其值为Black,White
        enum m_theme{Black ,White};
        //声明软件的语言为枚举类型,其值为CN,EN
        enum m_lang{CN,EN};
        //声明设备的模式为枚举类型,其值为 Simulator, SingleLane, DualLane
        enum m_laneMode{Simulator,SingleLane,DualLane};
        //声明设备的类型为枚举类型,其值为SPI,AOI
        enum m_machineType{SPI,AOI};

        //声明定义公司名称为字符型指针数组,分别为Scihet,Sung
        const char * m_pCompanyName[2] = {"Scijet","Sung"};
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    };
} //End of namespace AppSetting

#endif // APPSETTING_HPP
