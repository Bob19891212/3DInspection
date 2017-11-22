#include "measuredobj.hpp"

using namespace Job;

//>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//析构 & 构造函数
//初始化成员变量
MeasuredObj::MeasuredObj()
{
    //初始化元件名,将其设为空
    this->m_name = "";
    //初始化指向上一个MeasuredOb对象的指针,将其置为nullptr
    this->m_pNextMeasuredObj = nullptr;
    //初始化指向下一个MeasuredOb对象的指针,将其置为nullptr
    this->m_pPreMeasuredObj = nullptr;
}

MeasuredObj::~MeasuredObj()
{
    //将指向指向上一个与指向下一个MeasuredObj对象的指针置为nullptr
    this->m_pNextMeasuredObj = nullptr;
    this->m_pPreMeasuredObj = nullptr;
}
//<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SSDK::Rectangle MeasuredObj::getRectangle()
{
    //获取 rectangle 的X轴,Y轴坐标,角度,长和宽
    this->m_rectangle.getX();
    this->m_rectangle.getY();
    this->m_rectangle.getAngle();
    this->m_rectangle.getWidth();
    this->m_rectangle.getHeight();
    return this->m_rectangle;
}

void MeasuredObj::setRectangle()
{
    //设置rectangle的X轴,Y轴坐标,角度,长和宽
    this->m_rectangle.setX(1);
    this->m_rectangle.setY(1);
    this->m_rectangle.setAngle(1);
    this->m_rectangle.setWidth(1);
    this->m_rectangle.setHeight(1);
}
//<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

