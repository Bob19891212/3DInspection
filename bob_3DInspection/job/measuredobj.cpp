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
    this->m_pNextMeasuredObj = nullptr;
    this->m_pPreMeasuredObj = nullptr;
}
//<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
