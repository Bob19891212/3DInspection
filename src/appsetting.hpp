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
    class AppSetting
    {
    public:
        enum theme{Black ,White};
        enum lang{CN,EN};
        enum laneMode{Simulator,SingleLane,DualLane};
        enum machineType{SPI,AOI};

        const char * pCompanyName[2] = {"Scijet","Sung"};

        void readAppSetting(const QString& path);
        void writeAppSetting(const QString& path);
    };
} //End of namespace AppSetting

#endif // APPSETTING_HPP
