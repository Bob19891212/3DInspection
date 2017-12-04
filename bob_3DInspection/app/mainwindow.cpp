#include "mainwindow.hpp"

using namespace std;
using namespace Job;
using namespace App;
using namespace SSDK;
using namespace SSDK::DB;

MainWindow::MainWindow()
{

}

MainWindow::~MainWindow()
{

}

//>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//加载检测程序
void MainWindow::loadJob(QString path)
{
    //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // step1
    //实例化类对象(InspectionData,Board,MeasuredObjList三个类对象)
    InspectionData inspectionData;          //将 InspectionData 类实例化
    Board board;                            //将 board 类实例化
    MeasuredObjList measuredObjList;        //将检测对象列表实例化

    //将board的成员变量(指向检测对象列表的指针)指向实例化的检测对象列表
    board.setMeasurdObjList(&measuredObjList);
    //将inspectionData的成员变量(指向board信息的指针)指向实例化的board
    inspectionData.setBoard(&board);
    //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //step2
    //扫描目录下的检测程式
    //如果目录不存在,则抛出异常
    QDir dir(path);

    if(!dir.exists())
    {
        THROW_EXCEPTION("读取检测程式的文件路径不存在!!!");
    }
    //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //step3
    //过滤目录下所有的xml文件
    QStringList filters;            //创建过滤器列表，过滤xml为后缀的文件
    filters << "*[^xml]";           //"*[^x|^t][^m|^x][^l|^t]";
    dir.setNameFilters(filters);    //过滤目录下的xml文件

    //扫描目录下的文件
    dir.setFilter(QDir::Files);
    //获取文件目录下排除.xml文件后的文件列表
    QFileInfoList list = dir.entryInfoList();
    //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //step4
    //判断目录下是否存在检测程式
    //如果目录下没有检测程式,则随机生成一个检测程式数据
    //将检测程式数据写到检测程式文件中(sqlite数据库)
    //将检测程式数据写入到xml文件中
    //将检测程式数据显示在终端上
    //step4.1
    //如果目录下没有检测程式
    if ( 0 == list.size() )
    {
        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        // step4.1.1 随机生成一笔检测程式数据
        DataGeneration generator;           //实例化一个生成随机检测程式对象
        MeasuredObj objArr[OBJ_CNT];        //创建一个存放检测对象数据的数组
        //随机一笔检测程式数据生成数据
        generator.generateInspectionData(OBJ_CNT,
                                         &inspectionData,
                                         objArr);
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //step4.1.2 将检测程式数据写入到检测程式文件中(sqlite数据库)
        //2017.12.02 bob
        //将生成检测程式的名称改成V2
        writeInspectionDataToJob(path.toStdString()+"V2",
                                 &inspectionData);
        //step4.1.3 将检测程式数据写入xml文档
        inspectionData.writeInspectionDataToXml(path + "V2.xml");
        //step4.1.4 将检测对象数据在终端上显示
        inspectionData.pBoard()->pMeasuredObjList()->print();
    }
    //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //step4.2
    // 如果目录下有检测程式
    //将目录中检测程式文件显示在终端上,供用户选择
    //根据用户选择的检测程式文件(sqlite数据库),读取检测程式数据
    //将检测程式数据写入到xml文件中
    //将检测对象的数据显示在终端上
    else
    {
        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //step 4.2.1
        //在终端上显示ID号和文件名称,供用户选择
        cout << "ID Filename" << endl;
        //将文件及文件的序号显示在终端上
        for (int i = 0; i < list.size(); ++i)
        {
            QFileInfo fileInfo = list.at(i);

            cout << qPrintable(QString("%1  %2")
                               .arg(i + 1)
                               .arg(fileInfo.fileName()));
            cout << endl;
        }
        //选择对应检测程式文件的索引号
        int jobIndex{0};

        //在终端上显示提示信息,让用户输入检测程式的索引号
        cout << endl;
        cout << "请输入文件的ID号 (1-" << list.size()<<"):";
        cin >> jobIndex;

        //判断用户输入的索引号是否符合要求
        //判断用户输入是否为整数,如果不是整数,则输出错误提示,并重新输入
        //如果输入为整数,判断输入的整数是否匹配索引号
        //若果输入值与文件索引号不匹配,则输出错误提示,并重新输入
        //如果输入值与文件索引号匹配,则跳出循环
        while (true)
        {
            if( (jobIndex - (int)jobIndex) == 0 )
            {

                if(jobIndex > 0 && jobIndex <= list.size())
                {
                    break;
                }
            }
            cout << "输入非法数字,请重新输入: ";
            std::cin.clear();
            std::cin.ignore(numeric_limits<streamsize>::max(),'\n');
            jobIndex = 0;
            cin >> jobIndex;
        }
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //step2.4.2
        //获取用户选择的检测程式路径
        QString file = list.at(jobIndex-1).filePath();
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //step2.4.3
        //读取检测程式数据
        SqliteDB sqlite;
        sqlite.open(file.toStdString());             //打开检测程式文件
        //获取检测程式中检测对象的数量
        string sqlQuery = "SELECT COUNT(*) FROM MeasuredObjList";
        sqlite.prepare(sqlQuery);
        int objCnt = sqlite.executeScalar<int>(sqlQuery);

        //根据检测程式中检测对象的数量新建一个存放检测对象数据的数组
        MeasuredObj measuredObjArr[objCnt];

        //将检测程式中的数据读取到内存中
        readInspectionDataFromJob(objCnt,
                                  &inspectionData,
                                  measuredObjArr,
                                  &sqlite);
        sqlite.close();
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //step2.4.4
        //将检测程式数据写入到xml文件中
        inspectionData.writeInspectionDataToXml(file+".xml");

        //step2.4.5 将所有检测对象的数据显示在终端上
        inspectionData.pBoard()->pMeasuredObjList()->print();
    }
    //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
}
//<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//将inspectionData数据写入到检测程序中
void MainWindow::writeInspectionDataToJob(string path,
                                          InspectionData *pInspectionData)
{
    try
    {
        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //step1
        //在指定路径下创建数据库并打开
        SqliteDB sqlite;                  //SqliteDB类实例化一个对象
        sqlite.open(path);                //根据指定路径打开sqlite数据库
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //step2
        //创建Job表,并在表中添加"Version"和"LastEditingTime"两个字段
        //将相应的数据写入数据库
        string sqlCreate = "CREATE TABLE Job(Version TEXT,LastEditingTime TEXT);";
        sqlite.execute(sqlCreate);

        //插入Version数据及LastEditingTime字段数据
        string sqlInsert = "INSERT INTO Job(Version,LastEditingTime) VALUES(?,?);";
        sqlite.execute(sqlInsert,pInspectionData->version(),
                       pInspectionData->lastEditingTime());
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //step3
        //创建Board表,并在表中添加"Name","OriginalX","OriginalY","SizeX","SizeY"字段
        //并将相应的数据写入数据库
        sqlCreate = "CREATE TABLE Board(Name TEXT,OriginalX REAL,OriginalY REAL,SizeX REAL,SizeY REAL);";
        sqlite.execute(sqlCreate);

        //插入Board表中Name,OriginalX,OriginalY,SizeX,SizeY字段的数据
        sqlInsert = "INSERT INTO Board(Name,OriginalX,OriginalY,SizeX,SizeY) VALUES(?,?,?,?,?);";
        sqlite.execute(sqlInsert,pInspectionData->pBoard()->name(),
                       pInspectionData->pBoard()->originalX(),
                       pInspectionData->pBoard()->originalY(),
                       pInspectionData->pBoard()->sizeX(),
                       pInspectionData->pBoard()->sizeY());
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //step4
        //4.1创建MeasuredObjList表,并在表中添加"Name","PosX","PosY"
        //"Width","Height"字段,并将列表中数据写入到数据库中
        //2017.12.02 bob
        //添加写入检测对象的角度数据
        sqlCreate = "CREATE TABLE MeasuredObjList(Name TEXT,PosX REAL,PosY REAL,Width REAL,Height REAL,Angle REAL);";
        sqlite.execute(sqlCreate);

        //4.2定义临时指针,取出MeasuredObjList中所有节点的数据,默认指向链表数据的头地址
        MeasuredObj * pTmpObj = pInspectionData->pBoard()->pMeasuredObjList()->pHead();

        //4.3将MeasuredObj中的数据插入到MeasuredObjList的表中
        sqlInsert = "INSERT INTO MeasuredObjList(Name,PosX,PosY,Width,Height,Angle) VALUES(?,?,?,?,?,?);";
        //执行插入语句
        sqlite.prepare(sqlInsert);
        sqlite.begin();

        //将检测对象列表中索引数据插入到数据库的 MeasuredObjList 表中
        //2017.12.02 bob 添加写入检测对象的角度数据
        while (nullptr != pTmpObj)
        {
            string str = pTmpObj->name();
            sqlite.executeWithParms(str.data(),
                                    pTmpObj->rectangle().xPos(),
                                    pTmpObj->rectangle().yPos(),
                                    pTmpObj->rectangle().width(),
                                    pTmpObj->rectangle().height(),
                                    pTmpObj->rectangle().angle());
            //获取下一个检测对象的地址
            pTmpObj = pTmpObj->pNextMeasuredObj();
        }
        sqlite.commit();                      //将数据列表中数据一次性写入数据库
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //5.关闭数据库
        sqlite.close();
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    }
    catch (const exception &ex)
    {
        THROW_EXCEPTION(ex.what());
    }
}

void MainWindow::readInspectionDataFromJob(int objCnt,
                                           InspectionData * pInspectionData,
                                           MeasuredObj measuredObjArr[],
                                           SqliteDB * sqlite)
{
    try
    {
        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //step1
        //2017.12.02
        //读取检测程式的版本号
        //如果检测程式的版本号与当前软件版本一致,则读取检测程式数据
        //如果检测程式版本号是更低版本,则将低版本的程式转换为与当前软件匹配的版本
        //否则抛出异常
        string selectedString = "select Version from Job";
        sqlite->prepare(selectedString);
        string version = sqlite->executeScalar<string>(selectedString);

        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        if("V2" != version )
        {
            if( "V1"  == version )
            {
                version = "V2";
                convertJobToV2(sqlite);
            }
            else
            {
                THROW_EXCEPTION("检测程式版本有误,请确认!!");
            }
        }
        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //step1.1
        //获取检测程式的版本号
        pInspectionData->setVersion(version);     //获取检测程式版本号

        //获取检测程式的最后一次编辑时间
        selectedString = "select LastEditingTime from Job";
        sqlite->prepare(selectedString);
        string lastEditingTime = sqlite->executeScalar<string>(selectedString);
        pInspectionData->setLastEditingTime(lastEditingTime);
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //step1.2
        //获取基板数据,(基板的名称,基板的x,y原点坐标,基板的长,基板的宽)
        //获取基板的名称
        selectedString = "select Name from Board";
        sqlite->prepare(selectedString);
        string boardName = sqlite->executeScalar<string>(selectedString);
        pInspectionData->pBoard()->setName(boardName);
        //获取基板原点x轴坐标
        selectedString = "select OriginalX from Board";
        sqlite->prepare(selectedString);
        double originalX = sqlite->executeScalar<double>(selectedString);
        pInspectionData->pBoard()->setOriginalX(originalX);
        //获取基板原点y轴坐标
        selectedString = "select OriginalY from Board";
        sqlite->prepare(selectedString);
        double originalY = sqlite->executeScalar<double>(selectedString);
        pInspectionData->pBoard()->setOriginalY(originalY);
        //获取基板的长
        selectedString = "select SizeX from Board";
        sqlite->prepare(selectedString);
        double sizeX = sqlite->executeScalar<double>(selectedString);
        pInspectionData->pBoard()->setSizeX(sizeX);
        //获取基板的宽
        selectedString = "select SizeY from Board";
        sqlite->prepare(selectedString);
        double sizeY = sqlite->executeScalar<double>(selectedString);
        pInspectionData->pBoard()->setSizeY(sizeY);
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //step1.3
        //获取已检测对象的数据,具体数据如下:
        //被检查对象的名称,x,y轴坐标,宽和高
        //2017.12.02 bob 添加数据被检测对象的角度
        selectedString = "select * from MeasuredObjList";
        sqlite->prepare(selectedString);

        for (int i = 0; i < objCnt; ++i)
        {
            sqlite->step();
            auto name  = sqlite->columnValue(0);     //检测对象的名称
            measuredObjArr[i].setName((string)boost::get<string>(name));
            auto xPos = sqlite->columnValue(1);      //检测对象x轴坐标
            auto yPos = sqlite->columnValue(2);      //检测对象y轴坐标
            auto width = sqlite->columnValue(3);     //被检测对象的宽
            auto height = sqlite->columnValue(4);    //被检测对象的高
            //2017.12.02  bob 添加检测对象的角度
            auto angle = sqlite->columnValue(5);

            auto rect = Rectangle((double)boost::get<double>(xPos),
                                  (double)boost::get<double>(yPos),
                                  (double)boost::get<double>(width),
                                  (double)boost::get<double>(height),
                                  (double)boost::get<double>(angle));
            //设置检测对象的名称
            measuredObjArr[i].setRectangle(&rect);
            //将检测对象添加到链表的尾部
            pInspectionData->pBoard()->pMeasuredObjList()->pushTail(&measuredObjArr[i]);
        }
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    }
    catch(const exception &ex)
    {
        THROW_EXCEPTION(ex.what());
    }
}

//201712.02 bob
//添加检测程式格式转换
void MainWindow::convertJobToV2(SqliteDB *sqlite)
{
    //将检测程式的的版本号修改为"V2"
    string sqlUpdate = "UPDATE Version SET Version='V2'" ;
    sqlite->execute(sqlUpdate);

    //将所有的检测对象添加Angle字段,并设置默认值为0
    string addColumn = "ALTER TABLE MeasuredObjList ADD Angle REAL DEFAULT 0";
    sqlite->execute(addColumn);
}
//<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
