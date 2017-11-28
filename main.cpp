#include <iostream>

#include "app/app.hpp"
#include "job/inspectiondata.hpp"
#include "app/datageneration.hpp"

using namespace std;
using namespace App;
using namespace Job;
using namespace SSDK;

#define LIST_SIZE 50

int main()
{
    Config config;
    config.readSetting();

    MeasuredObj measuredObjArr[LIST_SIZE];
    MeasuredObjList measuredObjList;
    DataGeneration dataGeneration;
    dataGeneration.generateData(LIST_SIZE,measuredObjArr);


    for (int i = 0; i < LIST_SIZE; ++i)
    {
        measuredObjList.pushTail(&measuredObjArr[i]);
    }

    Board board;
    board.setMeasurdObjList(&measuredObjList);
    board.setName("Iphone");
    board.setOriginalX(1);
    board.setOriginalY(1);
    board.setSizeX(1);
    board.setSizeY(1);

    InspectionData inspectionData;
    inspectionData.setBoard(&board);
    inspectionData.setLastEditingTime("2017-11-27");
    inspectionData.setVersion("V2");

    inspectionData.pBoard()->measuredObjList()->print();
    inspectionData.writeInspectionDataToXml("test.xml");

    return 0;
}
