#include <iostream>

#include "sdk/customexception.hpp"
#include "app/appsetting.hpp"
#include "app/capturesetting.hpp"
#include "app/setting.hpp"
#include "job/measuredobjlist.hpp"
#include "job/board.hpp"
#include "job/inspectiondata.hpp"

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

//    MeasuredObjList measureObjList;
//    measureObjList.createLinkedList(50);
//    measureObjList.print();
//    measureObjList.pullHead();
//    int i;
//    i = measureObjList.getSize();
//    cout<<i<<endl;

    QString path ="test.xml";

    Job::Board board;
    board.randomObjListData();
    board.setName("apple");
    board.setSizeX(50);
    board.setSizeY(40);
    board.setOriginalX(20);
    board.setOriginalY(10);

    Job::InspectionData inspectionData;

    inspectionData.setBoard(&board);

    inspectionData.writeToXml(path);
    inspectionData.getBoard()->getMeasuredObjList().print();



    return 0;
}
