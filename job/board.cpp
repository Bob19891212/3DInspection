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
            QDomElement objList = pcbInfo.createElement("Target");
            board.appendChild(objList);

            QDomElement objName = pcbInfo.createElement(QString::fromLocal8Bit("元件名称"));
            QDomElement objX = pcbInfo.createElement(QString::fromLocal8Bit("X轴坐标"));
            QDomElement objY = pcbInfo.createElement(QString::fromLocal8Bit("Y轴坐标"));
            QDomElement objAngle = pcbInfo.createElement(QString::fromLocal8Bit("元件角度"));
            QDomElement objWidth = pcbInfo.createElement(QString::fromLocal8Bit("元件宽度"));
            QDomElement objHeight = pcbInfo.createElement(QString::fromLocal8Bit("元件高度"));

            objList.appendChild(objName);
            objList.appendChild(objX);
            objList.appendChild(objY);
            objList.appendChild(objAngle);
            objList.appendChild(objWidth);
            objList.appendChild(objHeight);

            QDomText objNameText = pcbInfo.createTextNode(QString::fromStdString(pTmpObj->getName()));
            QDomText objXText = pcbInfo.createTextNode(QString::number(pTmpObj->getRectangle().getX()));
            QDomText objYText = pcbInfo.createTextNode(QString::number(pTmpObj->getRectangle().getY()));
            QDomText objAngleText = pcbInfo.createTextNode(QString::number(pTmpObj->getRectangle().getAngle()));
            QDomText objWidthText = pcbInfo.createTextNode(QString::number(pTmpObj->getRectangle().getWidth()));
            QDomText objHeightText = pcbInfo.createTextNode(QString::number(pTmpObj->getRectangle().getHeight()));

            objName.appendChild(objNameText);
            objX.appendChild(objXText);
            objY.appendChild(objYText);
            objAngle.appendChild(objAngleText);
            objWidth.appendChild(objWidthText);
            objHeight.appendChild(objHeightText);

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


void Board::setMeasurdObjList(MeasuredObjList measuredObjList)
{
   this->m_measuredObjList = measuredObjList;
}

MeasuredObjList &Board::getMeasuredObjList()
{
    return m_measuredObjList;
}

//<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
