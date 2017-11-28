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
void Board::writeBoardDataToXml(QDomDocument inspectionData,QDomElement jobInfo)
{
    try
    {
        //在 inspectionData对象中创建一个board元素
        QDomElement board = inspectionData.createElement("Board");
        //设置board元素的属性
        board.setAttribute("基板名称",QString::fromStdString( this->name()));
        board.setAttribute("基板长度",this->sizeX());
        board.setAttribute("基板宽度",this->sizeY());
        board.setAttribute("X轴坐标",this->originalX());
        board.setAttribute("Y轴坐标",this->originalY());

        //将board元素添加至jobInfo节点下
        jobInfo.appendChild(board);

        //定义一个临时对象指针,指向链表的头部
        MeasuredObj * pTmpObj = this->m_pMeasuredObjList->pHead();

        //将链表中所有元素
        while (pTmpObj != nullptr)
        {
            //在inspectionData文档中添加元素 measuredObj(元素为元件名称)
            QDomElement measuredObj = inspectionData.createElement(QString::fromStdString(pTmpObj->name()));

            //设置元件名称的属性(x,y轴坐标,角度,长和宽)
            QString str = QString::number(pTmpObj->rectangle().xPos());
            measuredObj.setAttribute("X轴坐标",str);
            str = QString::number(pTmpObj->rectangle().yPos());
            measuredObj.setAttribute("Y轴坐标",str);
            str = QString::number(pTmpObj->rectangle().angle());
            measuredObj.setAttribute("元件角度",str);
            str = QString::number(pTmpObj->rectangle().width());
            measuredObj.setAttribute("元件宽度",str);
            str = QString::number(pTmpObj->rectangle().height());
            measuredObj.setAttribute("元件高度",str);
            //将 measuredObj中的元素添加到board节点下
            board.appendChild(measuredObj);
            //获取下一个节点的地址
            pTmpObj = pTmpObj->pNextMeasuredObj();
        }
    }
    catch(const exception &ex)
    {
        THROW_EXCEPTION(ex.what());
    }
}
//<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
