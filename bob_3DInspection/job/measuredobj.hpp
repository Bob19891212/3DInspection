#ifndef MEASUREDOBJ_HPP
#define MEASUREDOBJ_HPP

#include "../sdk/rectangle.hpp"
#include "../sdk/customexception.hpp"

using namespace std;

namespace Job
{
    /**
     *  @brief MeasuredObj
     *         获取当前被检测对象的名称,及类型,
     *         获取上一个及下一个被检测对象数据的头指针
     *  @author bob
     *  @version 1.00 2017-11-22 bob
     *                note:create it
     */
    class MeasuredObj
    {
    public:
        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //析构 & 构造 函数
        //初始化成员变量
        MeasuredObj();

        //释放成员变量(指针)
        ~MeasuredObj();
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //访存函数
        //设置&获取下一个对象的头指针
        void setNextMeasuredObjPtr(Job::MeasuredObj *ptr)
        {
            this->m_pNextMeasuredObj = ptr;
        }
        Job::MeasuredObj * pNextMeasuredObj()
        {
            return this->m_pNextMeasuredObj;
        }

        //设置&获取上一个对象的头指针
        void setPreMeasuredObjPtr(Job::MeasuredObj *ptr)
        {
            this->m_pPreMeasuredObj = ptr;
        }
        Job::MeasuredObj * pPreMeasuredObj()
        {
            return this->m_pPreMeasuredObj;
        }

        //设置及获取元件的名称
        void setName(std::string name){this->m_name = name;}
        std::string name(){return this->m_name;}

        //设置&获取元件对象的X,Y轴坐标,角度,宽和高
        void setRectangle(SSDK::Rectangle * rectangle)
        {
            this->m_rectangle = (*rectangle);
        }
        SSDK::Rectangle & rectangle() {return this->m_rectangle;}
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    private:
        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //成员变量
        Job::MeasuredObj *m_pNextMeasuredObj;    //上一个被检测对象的头指针
        Job::MeasuredObj *m_pPreMeasuredObj;     //下一个被检测对象的头指针
        std::string m_name;                      //当前被检测对象的名称
        SSDK::Rectangle m_rectangle;             //实例化一个Rectangle对象
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    };
} //End of namespace Job

#endif // MEASUREDOBJ_HPP
