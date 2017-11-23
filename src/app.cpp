#include <iostream>

#include "sdk/customexception.hpp"
#include "appsetting.hpp"
#include "capturesetting.hpp"
#include "setting.hpp"
#include "../job/measuredobjlist.hpp"

using namespace std;
using namespace App;
using namespace Job;


int main()
{
//    AppSetting appSetting;
//    appSetting.readAppSetting("app.ini");

//    CaptureSetting captureSetting;
//    captureSetting.writeCaptureSetting("CaptureSetting.ini");
//    captureSetting.readCaptureSetting("CaptureSetting.ini");

<<<<<<< HEAD


    Setting setting;
    setting.readSetting();
=======
    MeasuredObjList measureObjList;
    measureObjList.createLinkedList(50);
    measureObjList.print();
    measureObjList.pullHead();
    int i;
    i = measureObjList.getSize();
    cout<<i<<endl;
>>>>>>> feature_measuredobjlist

    return 0;
}
