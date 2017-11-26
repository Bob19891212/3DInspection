#include "board.hpp"


using namespace std;
using namespace Job;

//>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//构造 & 析构函数
Board::Board()
{
    //初始化成员变量
    this->m_name = "";
    this->m_sizeX = 0;
    this->m_sizeY = 0;
    this->m_originalX = 0;
    this->m_originalX = 0;
}

Board::~Board()
{

}

//<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//成员函数
void Board::writeToXml(QString path)
{
    try
    {
        QDomDocument pcbInfo;

        QDomElement board = pcbInfo.createElement("Board");

        board.setAttribute("基板名称",QString::fromStdString( this->getName()));

        board.setAttribute("基板长度",this->getSizeX());

        board.setAttribute("基板宽度",this->getSizeY());

        board.setAttribute("X轴坐标",this->getOriginalX());

        board.setAttribute("Y轴坐标",this->getOriginalY());

        pcbInfo.appendChild(board);

        MeasuredObj * pTmpObj = this->m_measuredObjList.getHeadPtr();

        while (pTmpObj != nullptr)
        {
            QDomElement objList = pcbInfo.createElement(QString::fromStdString(pTmpObj->getName()));

            board.appendChild(objList);

//            QString str = QString::fromStdString(pTmpObj->getName());
//            objList.setAttribute("元件名称",str);

            //            QDomAttr objProperty = pcbInfo.createAttribute("元件名称");
            //            objProperty.setValue(QString::fromStdString(pTmpObj->getName()));

            //            objList.setAttributeNode(objProperty);

            //            objProperty = pcbInfo.createAttribute("X轴坐标");
            //            objProperty.setValue(QString::number(pTmpObj->getRectangle().getX()));

            //            objList.setAttributeNode(objProperty);

            //            objProperty = pcbInfo.createAttribute("Y轴坐标");
            //            objProperty.setValue(QString::number(pTmpObj->getRectangle().getY()));

            //            objProperty = pcbInfo.createAttribute("元件角度");
            //            objProperty.setValue(QString::number(pTmpObj->getRectangle().getAngle()));


            //            objList.setAttributeNode(objProperty);


            QString str = QString::number(pTmpObj->getRectangle().getX());
            objList.setAttribute("X轴坐标",str);

            str = QString::number(pTmpObj->getRectangle().getY());
            objList.setAttribute("Y轴坐标",str);

            str = QString::number(pTmpObj->getRectangle().getAngle());
            objList.setAttribute("元件角度",str);

            str = QString::number(pTmpObj->getRectangle().getWidth());
            objList.setAttribute("元件宽度",str);

            str = QString::number(pTmpObj->getRectangle().getHeight());
            objList.setAttribute("元件高度",str);



            pTmpObj = pTmpObj->getNextMeasuredObjPtr();
        }





        QString filePath( path );
        QFile file(filePath);

        if (!file.open(QFile::WriteOnly | QFile::Text))
        {
            THROW_EXCEPTION("打开文件失败!!");
        }

        QTextStream out(&file);

        pcbInfo.save(out,4);

        file.close();
    }
    catch(const exception &ex)
    {
        THROW_EXCEPTION(ex.what());
    }

}

void Board::randomObjListData()
{
    this->m_measuredObjList.createLinkedList(SIZE);
}
//<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
