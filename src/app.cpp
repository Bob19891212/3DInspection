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

    MeasuredObjList measureObjList;
    measureObjList.createLinkedList(5);
    measureObjList.print();
    int i;
    i = measureObjList.getSize();
    cout<<"\n"<<i<<endl;

    return 0;
}
