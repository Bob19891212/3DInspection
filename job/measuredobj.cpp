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

}
//<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SSDK::Rectangle MeasuredObj::getRectangle()
{
    //获取 rectangle 的X轴,Y轴坐标,角度,长和宽
    return this->m_rectangle;
}

void MeasuredObj::setRectangle()
{
    try
    {
        //设置rectangle的X轴,Y轴坐标,角度,长和宽
        this->m_rectangle.setX(RANDOM_NUM);
        this->m_rectangle.setY(RANDOM_NUM);
        this->m_rectangle.setAngle(RANDOM_NUM);
        this->m_rectangle.setWidth(RANDOM_NUM);
        this->m_rectangle.setHeight(RANDOM_NUM);
    }
    catch(const exception &ex)
    {
        THROW_EXCEPTION(ex.what());
    }
}
//<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

