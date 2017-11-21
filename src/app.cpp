#include <iostream>

#include "sdk/customexception.hpp"
#include "appsetting.hpp"
#include "capturesetting.hpp"
#include "setting.hpp"

using namespace std;
using namespace App;


int main()
{
//    AppSetting appSetting;
//    appSetting.readAppSetting("app.ini");

//    CaptureSetting captureSetting;
//    captureSetting.writeCaptureSetting("CaptureSetting.ini");
//    captureSetting.readCaptureSetting("CaptureSetting.ini");

    Setting setting;
    setting.readAppSetting("app.ini");
    setting.readCaptureSetting("CaptureSetting.ini");

    return 0;
}
