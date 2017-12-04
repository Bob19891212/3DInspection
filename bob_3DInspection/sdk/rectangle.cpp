#include "rectangle.hpp"

using namespace SSDK;

Rectangle::Rectangle()
{
    //初始化成员变量,将变量值赋为0
    this->m_xPos = 0;
    this->m_yPos = 0;
    this->m_width = 0;
    this->m_height = 0;
}

Rectangle::Rectangle(double xPos,
                     double yPos,
                     double width,
                     double height,
                     double angle)
{
    this->m_xPos = xPos;
    this->m_yPos = yPos;
    this->m_width = width;
    this->m_height = height;
    this->m_angle = angle;          //2017.12.02 bob 添加成员变量rectangle的角度
}

Rectangle::~Rectangle()
{

}


