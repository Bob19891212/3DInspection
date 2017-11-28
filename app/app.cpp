#include "app.hpp"

using namespace std;
using namespace App;

void Config::readSetting()
{
    try
    {
        //如果存放配置文件路径的文件不存在,则将存放设置文件的路径写入默认值
        if(!QFile::exists(this->m_appConfig))
        {
            writeToAppConfig();
        }

        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //step1
        //读取文件Theme内容,将读取appsetting.ini和capturesetting.ini配置文件的路径
        QSettings configFile(this->m_appConfig,QSettings::IniFormat);
        this->m_appSettingPath = configFile.value("AppSettingPath").toString();
        this->m_captureSettingPath = configFile.value("CaptureSettingPath").toString();

        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //step2
        //读取appsetting.ini和capturesetting.ini配置文件的路径
        //将AppSetting类实例化,调用读取文件(app.ini)的成员函数
        AppSetting appSetting;
        appSetting.readAppSetting(this->m_appSettingPath);

        //将CaptureSetting类实例化,调用读取文件(capture.ini)的成员函数
        CaptureSetting captureSetting;
        captureSetting.readCaptureSetting(this->m_captureSettingPath);
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    }
    catch(const exception &ex)
    {
        //捕获异常时,将异常抛出
        THROW_EXCEPTION(ex.what());
    }
}

void Config::writeToAppConfig()
{
    //将类QSettings实例化一个对象
    //QSettings::IniFormat: 为存放文件的格式
    QSettings configFile(this->m_appConfig,QSettings::IniFormat);

    //将AppSetting.ini和CaptureSetting.ini文件的路径写入配置文件中
    configFile.setValue("AppSettingPath","./AppSetting.ini");
    configFile.setValue("CaptureSettingPath","./CaptureSetting.ini");
}

