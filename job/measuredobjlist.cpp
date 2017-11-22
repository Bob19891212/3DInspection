#include "measuredobjlist.hpp"

using namespace std;
using namespace Job;
using namespace SSDK;

//>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//构造 &析构函数
MeasuredObjList::MeasuredObjList()
{
    this->m_size = 0;
    this->m_pHeadObj = nullptr;
}

MeasuredObjList::~MeasuredObjList()
{
    //定义一个MeasureObj对象,记录链表节点的地址
    MeasuredObj * pTmpObj = nullptr;

    //如果表头指针不为nullPtr,则依次释放所有链表的节点
    while (this->m_pHeadObj != nullptr)
    {
        //将链表中下一个节点的地址赋值给对象
        pTmpObj = this->m_pHeadObj->getNextMeasuredObjPtr();

        //删除链表的头地址
        delete(this->m_pHeadObj);

        //如果链表中下一个节点的地址不是nullptr,即不是最后一个节点
        //将链表中指向上一个节点的地址置为nullptr
        if(pTmpObj != nullptr)
        {
            pTmpObj->setPreMeasuredObjPtr(nullptr);
        }
        //将指向下一个节点的地址赋值给表头
        this->m_pHeadObj = pTmpObj;
    }
}

//<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//创建链表,增加,删除节点
void MeasuredObjList::createLinkedList(int size)
{
    //创建两个MeasuredObj指针对象 pTailObj (为指向链表尾部节点的指针),将其置为nullptr
    //将 pNewObj(为指向链表新建节点的指针),将其置为nullptr
    MeasuredObj * pTailObj = nullptr;
    MeasuredObj * pNewObj = nullptr;

    //根据参数,开辟指定大小的链表
    for (int i = 0; i < size ; ++i)
    {
        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //step1
        //新增一个Measure对象
        pNewObj = new (MeasuredObj);

        //判断内存是否开辟成功,如果失败,则抛出异常
        if(nullptr == pNewObj)
        {
            THROW_EXCEPTION("内存分配失败!");
        }
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //step2
        //将新增的成员变量赋值
        //设置新增节点的X,Y轴坐标,角度,及长和宽
        pNewObj->setRectangle();

        //设置新增节点元件名称
        pNewObj->setName("chip001");
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //step3
        //设置链表当前节点的成员变量(即上一个节点地址及下一个节点的地址)
        //step3.1
        //判断链表新增节点是否为第一个节点,如果不是则有以下4步操作
        //1.将新增加的对象中的成员变量,指向下一个链表节点的指针置为nullptr
        //2.将新增加的对象中的成员变量,指向上一个链表节点的指针指向链表的尾部
        //3.将原来链表尾部中的成员变量指向下一个链表节点的指针指向当前新增的节点
        //4.将新增的对象赋值给链表尾部对象
        if(0 != i )
        {
             pNewObj->setNextMeasuredObjPtr(nullptr);
             pNewObj->setPreMeasuredObjPtr(pTailObj);
             pTailObj->setNextMeasuredObjPtr(pNewObj);
             pTailObj = pNewObj;
        }
        //step3.2
        //如果新增的节点为链表中第一个节点,则有如下步骤
        //1.将新增对象的指针赋值给成员变量 m_pHeadObj(即链表第一个节点的地址)
        //2.将链表的第一个节点中指向下一个节点的指针置为nullptr
        //3.将链表的第一个节点中指向上一个节点的指针置为nullptr
        //4.将指向链表第一个节点的指针赋值给指向链表尾部节点的指针
        else
        {
            this->m_pHeadObj= pNewObj;
            this->m_pHeadObj->setNextMeasuredObjPtr(nullptr);
            this->m_pHeadObj->setPreMeasuredObjPtr(nullptr);
            pTailObj = this->m_pHeadObj;
        }
        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    }
}

void MeasuredObjList::pushHead()
{
    MeasuredObj * pNewObj = nullptr;
    MeasuredObj *pTmpObj = this->m_pHeadObj;

    pNewObj = new (MeasuredObj);

    if(nullptr == pNewObj)
    {
        THROW_EXCEPTION("内存开辟失败!");
    }

    pNewObj->setRectangle();

    pNewObj->setName("U1");

    this->m_pHeadObj = pNewObj;
    this->m_pHeadObj->setNextMeasuredObjPtr(pTmpObj);
    this->m_pHeadObj->setPreMeasuredObjPtr(nullptr);





}

void MeasuredObjList::pullHead()
{
    //定义一个MeasureObj对象,记录链表节点的地址
    MeasuredObj *pTmpObj = nullptr;
    //将链表中第二个节点的地址赋值给临时对象
    pTmpObj = this->m_pHeadObj->getNextMeasuredObjPtr();

    //删除链表的头
    delete(this->m_pHeadObj);

    //如果链表中下一个节点的地址不是nullptr,即不是最后一个节点
    //将链表中指向上一个节点的地址置为nullptr
    if(pTmpObj != nullptr)
    {
        pTmpObj->setPreMeasuredObjPtr(nullptr);
    }
    //将指向下一个节点的地址赋值给表头
    this->m_pHeadObj = pTmpObj;
}

void MeasuredObjList::pushTail()
{

}

void MeasuredObjList::pullTail()
{

}

//<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


int MeasuredObjList::getSize()
{
    //定义一个MeasureObj对象,记录链表的头指针
    MeasuredObj *pTmpObj = this->m_pHeadObj;

    //依次轮询链表中的每个节点中成员变量(指向下一个节点的指针),
    //链表节点中成员变量(指向下一个节点的指针)为nullptr则跳出循环
    //记录下链表中节点的个数
    while (pTmpObj != nullptr)
    {
        this->m_size++;
        pTmpObj = pTmpObj->getNextMeasuredObjPtr();
    }
    //返回链表节点的个数
    return this->m_size;
}

void MeasuredObjList::print()
{
    MeasuredObj *pTmpObj = this->m_pHeadObj;

    while (pTmpObj != nullptr)
    {
        printf("%f ", pTmpObj->getRectangle().getX());
        pTmpObj = pTmpObj->getNextMeasuredObjPtr();
    }
}


