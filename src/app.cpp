#include <iostream>

#include "sdk/customexception.hpp"
#include "appsetting.hpp"
#include "capturesetting.hpp"

using namespace std;
using namespace App;


int main()
{
    AppSetting appSetting;
	  appSetting.readAppSetting("app.ini");
    CaptureSetting captureSetting;
    captureSetting.writeCaptureSetting("CaptureSetting.ini");



    return 0;
}
