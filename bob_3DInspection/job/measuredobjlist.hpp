#ifndef MEASUREDOBJLIST_HPP
#define MEASUREDOBJLIST_HPP

#include <ctime>

#include "../sdk/customexception.hpp"
#include "../sdk/formatconvertion.hpp"
#include "measuredobj.hpp"

template <class T>
class MeasuredObjList
{
public:
    void pushHead(T *pMeasuredObj);

    void pushTail(T *pMeasuredObj);

    void pullHead();

    void pullTail();

    T * pHead();

    void print();

private:
    T *m_pHeadObj;
    int m_size;
};

#endif // MEASUREDOBJLIST_HPP


template<class T>
void MeasuredObjList<T>::pushHead(T *pMeasuredObj)
{
    try
    {
        T * pTmpObj = this->m_pHeadObj;  //pTmpObj为临时记录列表检测对象的地址

        //设置对象的成员变量(指向上一个检测对象的指针),设置为nullptr
        pMeasuredObj->setNextMeasuredObjPtr(nullptr);
        //设置对象的成员变量(指向下一个检测对象的指针)指向原来的表头
        pMeasuredObj->setNextMeasuredObjPtr(pTmpObj);

        //如果列表原来的头指针不为nullptr,则设置原来表头上一个头指针指向当前的表头
        if(nullptr != pTmpObj)
        {
            pTmpObj->setPreMeasuredObjPtr(pTmpObj);
        }

        this->m_pHeadObj = pMeasuredObj;                //重新设置列表的头指针

        this->m_size++;                                 //将列表的长度 +1
    }
    catch(const exception &ex)
    {
        THROW_EXCEPTION(ex.what());
    }
}

template<class T>
void MeasuredObjList<T>::pushTail(T *pMeasuredObj)
{
    try
    {
        T * pTailObj = this->m_pHeadObj; //pTailObj:为记录列表尾部检测对象的指针
        T * pTmpObj;                   //pTmpObj:为临时记录列表检测对象地址的指针

        if(nullptr != this->m_pHeadObj)
        {
            pTmpObj = this->m_pHeadObj->pNextMeasuredObj();
        }
        else
        {
            pTmpObj = nullptr;
        }

        while (nullptr != pTmpObj)
        {
            pTailObj = pTmpObj;
            pTmpObj = pTmpObj->pNextMeasuredObj();
        }

        // 设置对象中成员变量(指向下一个检测对象的指针)设置为nullptr
        pMeasuredObj->setNextMeasuredObjPtr(nullptr);
        // 将新创建对象中成员变量(指向下一个检测对象的指针)设置为nullptr
        pMeasuredObj->setPreMeasuredObjPtr(pTailObj);

        //如果列表尾部不为nullptr,则将列表尾部成员变量(指向下一个检测对象的指针)指向列表尾部
        if(nullptr != pTailObj)
        {
            pTailObj->setNextMeasuredObjPtr(pMeasuredObj);
        }
        else
        {
            this->m_pHeadObj = pMeasuredObj;
        }
        this->m_size++;                             //将列表的长度 +1
    }
    catch(const exception &ex)
    {
        THROW_EXCEPTION(ex.what());
    }
}

template<class T>
void MeasuredObjList<T>::pullHead()
{
    try
    {
        if(this->m_size > 0)
        {
            T *pTmpObj = nullptr;
            pTmpObj = this->m_pHeadObj->pNextMeasuredObj();
            this->m_pHeadObj->setNextMeasuredObjPtr(nullptr);

            if(pTmpObj != nullptr)
            {
                pTmpObj->setPreMeasuredObjPtr(nullptr);
            }

            this->m_pHeadObj = pTmpObj;

            this->m_size--;                     //将列表中的长度减一
        }
        else
        {
            THROW_EXCEPTION("列表长度为0,无法删除检测对象!");
        }
    }
    catch(const exception &ex)
    {
        THROW_EXCEPTION(ex.what());
    }
}

template<class T>
void MeasuredObjList<T>::pullTail()
{
    try
    {
        if(this->m_size > 0)
        {
            T * pTailObj = this->m_pHeadObj;
            T * pTmpObj = this->m_pHeadObj->pNextMeasuredObj();

            while (pTmpObj != nullptr)
            {
                pTailObj = pTmpObj;
                pTmpObj = pTmpObj->pNextMeasuredObj();
            }

            pTmpObj = pTailObj->pPreMeasuredObj();

            pTailObj->setPreMeasuredObjPtr(nullptr);
            pTailObj = nullptr;

            if(pTmpObj != nullptr)
            {
                //将倒数第二个检测对象中的成员变量(指向下一个检测对象的指针)
                pTmpObj->setNextMeasuredObjPtr(nullptr);
            }
            else
            {
                this->m_pHeadObj = nullptr;
            }

            this->m_size--;                     //将列表的长度减一
        }
        else
        {
            THROW_EXCEPTION("列表长度为0,无法删除检测对象!");
        }
    }
    catch(const exception &ex)
    {
        THROW_EXCEPTION(ex.what());
    }
}

template<class T>
T *MeasuredObjList<T>::pHead()
{
    return this->m_pHeadObj;
}

template<class T>
void MeasuredObjList<T>::print()
{
    try
    {
        T *pTmpObj = this->m_pHeadObj;

        while (pTmpObj != nullptr)
        {
            cout<<"Name:"<<pTmpObj->name()<<"\t"
               <<"X:"<< pTmpObj->rectangle().xPos()<<"\t"
              <<"Y:"<<pTmpObj->rectangle().yPos()<<"\t"
             <<"Width:"<<pTmpObj->rectangle().width()<<"\t"
            <<"Height:"<<pTmpObj->rectangle().height()<<"\t"
            <<"Angle:"<<pTmpObj->rectangle().angle()<<endl;
            pTmpObj = pTmpObj->pNextMeasuredObj();
        }
    }
    catch(const exception &ex)
    {
        THROW_EXCEPTION(ex.what());
    }
}
