#ifndef BOARD_HPP
#define BOARD_HPP

#include <QDomDocument>
#include <QFile>
#include <QTextStream>

#include "measuredobjlist.hpp"

#define SIZE 50

using namespace std;

namespace Job
{
    /**
     *  @brief Board
     *         获取一个整板PCB信息,具体信息如下:
     *         PCB名称,PCB的长宽,PCB的原点位置
     *         PCB上所有元件的名称,元件的X,Y坐标.角度,及长宽
     *         将PCB上所有信息输出到xml文件中
     *  @author bob
     *  @version 1.00 2017-11-23 bob
     *                note:create it
     */
    class Board
    {
    public:
        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //构造 & 析构函数
        //初始化成员变量
        Board();

        ~Board();
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //成员函数,将PCB信息输出值XML文件
        /*
        *  @brief  writeToXml
        *          将PCB数据写入至指定路径的xml文件中
        *  @param  path:将数据写入文件的路径
        *  @return  N/A
        */
        void writeToXml(QString path);

        /*
        *  @brief  randomBoardData
        *          随机生成一块PCB板上所有元件的名称,长,宽,及x,y轴坐标角度
        *  @param  N/A
        *  @return N/A
        */
        void randomObjListData();
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //访存函数
        //设置&获取PCB的名称
        void setName(std::string name){this->m_name = name;}

        std::string getName(){return this->m_name;}

        //设置&获取PCB的长
        void setSizeX(double sizeX) {this->m_sizeX = sizeX;}

        double getSizeX() { return this->m_sizeX;}

        //设置&获取PCB的宽
        void setSizeY(double sizeY) {this->m_sizeY = sizeY;}

        double getSizeY() { return this->m_sizeY;}

        //设置&获取PCB原点X轴的坐标
        void setOriginalX(double originalX) {this->m_originalX = originalX;}

        double getOriginalX() { return this->m_originalX;}

        //设置&获取PCB原点Y轴的坐标
        void setOriginalY(double originalY) {this->m_originalY = originalY;}

        double getOriginalY() { return this->m_originalY;}

        //设置&获取MeasuredObjList(即已检测对象)的链表
        void setMeasurdObjList(MeasuredObjList & measuredObjList)
        {
            this->m_measuredObjList = measuredObjList;
        }

        MeasuredObjList &getMeasuredObjList() {return this->m_measuredObjList;}
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    private:
        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //成员变量
        std::string m_name;                     //记录PCB的名称
        double m_sizeX;                         //记录PCB的长
        double m_sizeY;                         //记录PCB的宽
        double m_originalX;                     //记录PCB原点X轴的坐标
        double m_originalY;                     //记录PCB原点Y轴的坐标
        MeasuredObjList m_measuredObjList;      //实例化一个检测对象列表
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    };
}  //End of namespace Job


#endif // BOARD_HPP
