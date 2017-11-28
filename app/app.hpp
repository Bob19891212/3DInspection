#ifndef SETTING_HPP
#define SETTING_HPP

#include "appsetting.hpp"
#include "capturesetting.hpp"

using namespace std;
using namespace App;

namespace App
{
    /**
     *  @brief Setting
     *         读取appsetting和capturesetting的配置文件
     *  @author bob
     *  @version 1.00 2017-11-21 bob
     *                note:create it
     */
    class Config
    {
    public:
        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //成员函数
        /*
        *  @brief  读取所有的配置文件,("AppSetting.ini","CaptureSetting.ini")
        *  @param  N/A
        *  @return N/A
        */
        void readSetting();

        /*
        *  @brief  将"AppSetting.ini","CaptureSetting.ini"配置文件路径
        *          写入到appconfig文件中
        *  @param  N/A
        *  @return N/A
        */
        void writeToAppConfig();
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    private:
        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //成员变量,设置配置文件的路径
        QString m_appSettingPath;
        QString m_captureSettingPath;
        //存放配置文件路径的文件
        QString m_appConfig{"./AppConfig"};
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    };
}  //End of namespace App


#endif // SETTING_HPP
