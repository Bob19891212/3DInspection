#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include <iostream>

using namespace std;

namespace SSDK
{
    /**
     *  @brief Rectangle
     *         设置&获取元件的X,Y坐标及元件的长和宽的值
     *  @author bob
     *  @version 1.00 2017-11-21 bob
     *                note:create it
     */
    class Rectangle
    {
    public:
        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //构造 & 析构函数
        //初始化类的成员变量
        Rectangle();

        /*
        *  @brief  Rectangle
        *          根据参数,将其赋值给成员变量m_xPos,m_yPos,m_angle,m_width,m_height
        *  @param  xPos: 设置rectangle的x坐标
        *          yPos: 设置rectangle的y坐标
        *          width:设置rectangle的宽度
        *         height:设置rectangle的高度
        *         angle: 设置rectangle的angle角度         2017.12.02 bob
        *  @return  N/A
        */
        Rectangle(double xPos,
                  double yPos,
                  double width,
                  double height,
                  double angle);

        ~Rectangle();
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //访存函数,给类的成员变量赋值
        //给成员变量X赋值,获取元件的X轴坐标
        void setX(double x){ this->m_xPos = x;}
        double xPos(){ return this->m_xPos;}

        //给成员变量Y赋值,获取元件的Y轴坐标
        void setY(double y){ this->m_yPos = y;}
        double yPos(){ return this->m_yPos;}

        //给成员变量width赋值,获取元件的宽度
        void setWidth(double width){this->m_width = width;}
        double width(){ return this->m_width;}

        //给成员变量height赋值,获取元件的高度
        void setHeight(double height){this->m_height = height;}
        double height(){ return this->m_height;}

        //2017.12.02 bob
        //给成员变量angle赋值,获取元件的角度
        void setAngle(double angle){this->m_angle = angle;}
        double angle(){ return this->m_angle;}
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    private:
        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //成员变量
        double m_xPos;                           //元件X的坐标
        double m_yPos;                           //元件Y的坐标
        double m_width;                          //元件的宽度
        double m_height;                         //元件的高度
        double m_angle;                          // 2017.12.02 bob  元件的角度
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    };
}  //End of namespace SSDK

#endif // RECTANGLE_HPP
