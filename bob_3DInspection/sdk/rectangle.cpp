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
                     double height)
{
    this->m_xPos = xPos;
    this->m_yPos = yPos;
    this->m_width = width;
    this->m_height = height;
}

Rectangle::~Rectangle()
{

}


