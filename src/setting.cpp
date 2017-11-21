#include "setting.hpp"

using namespace std;
using namespace App;

void Setting::readSetting()
{
    try
    {
        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //将AppSetting类实例化,调用读取文件(app.ini)的成员函数
        AppSetting appSetting;
        appSetting.readAppSetting(this->appSettingPath);

        //将CaptureSetting类实例化,调用读取文件(capture.ini)的成员函数
        CaptureSetting captureSetting;
        captureSetting.readCaptureSetting(this->captureSettingPath);

        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    }
    catch(const exception &ex)
    {
        //捕获异常时,将异常抛出
        THROW_EXCEPTION(ex.what());
    }
}
