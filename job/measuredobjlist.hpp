#ifndef MEASUREDOBJLIST_HPP
#define MEASUREDOBJLIST_HPP

#include <ctime>

#include "../sdk/customexception.hpp"
#include "../datahelper/formatconvertion.hpp"
#include "measuredobj.hpp"

using namespace std;

namespace Job
{

    /**
     *  @brief MeasuredObjList
     *         根据指定长度,创建一个双向链表
     *         插入节点,删除节点,清空所有节点
     *         获取链表的长度
     *  @author bob
     *  @version 1.00 2017-11-22 bob
     *                note:create it
     */
    class MeasuredObjList
    {
    public:
        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //构造 & 析构函数
        //初始化成员变量
        MeasuredObjList();

        //释放链表存储空间
        ~MeasuredObjList();
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //成员函数
        //创建链表 ,添加&删除链表的节点
        /*
        *  @brief  createLinkedList
        *          根据长度,创建指定长度的被检查对象(MeasuredObj)的链表
        *  @param  size: 链表的长度(根据长度生成指定长度的链表)
        *  @return N/A
        */
        void createLinkedList(int size);

        //添加链表的节点
        /*
        *  @brief   将对象插入至链表的表头
        *  @param   measuredObj:要插入链表的measuredObj对象(即插入至链表的表头对象)
        *  @return  N/A
        */
        void pushHead(MeasuredObj measuredObj);

        /*
        *  @brief   将对象插入至链表的尾部
        *  @param   measuredObj:要插入链表的measuredObj对象(即插入至链表的尾部对象)
        *  @return  N/A
        */
        void pushTail(MeasuredObj measuredObj);

        /*
        *  @brief   删除链表中第一个节点
        *  @param   N/A
        *  @return  N/A
        */
        void pullHead();

        /*
        *  @brief   删除链表中最后一个节点
        *  @param   N/A
        *  @return  N/A
        */
        void pullTail();

        //获取链表的大小
        int getSize();

        //设置链表的大小
        void setSize(int size);

        //获取链表的头指针
        Job::MeasuredObj * getHeadPtr(){return this->m_pHeadObj;}
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //将链表中的数据输出至终端
        void print();
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    private:
        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //成员变量
        int m_size;                     //链表的大小
        Job::MeasuredObj * m_pHeadObj;  //指向链表表头的指针
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    };
} //End of namespace Job

#endif // MEASUREDOBJLIST_HPP