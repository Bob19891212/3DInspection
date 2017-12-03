#ifndef DATACONVERT_HPP
#define DATACONVERT_HPP

#include <vector>
#include <stdint.h>
#include <string>
#include <netinet/in.h>
#include <iostream>

#include "Exception/marcoexception.hpp"

namespace SSDK
{
    /**
     *  @brief DataConverter
     *
     *  @author rime
     *  @version 1.00 2017-07-19 rime
     *                note:create it
     *
     *     主要负责数据类型的转化, 主要场景有：
     *              1.在TCP中，需要其他类型和char类型的相互转化，会调用到这里的函数
     */
    class DataConverter
    {
    public:

        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //constructor & deconstructor

        DataConverter();

        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        //convertion functions

        /**
         * @brief copyToCharVector
         *             拷贝数据到一个vector对象
         * @param srcData
         *              源数据的vector对象
         * @param startIndex
         *              拷贝源数据的起始索引
         * @param dstData
         *              目标数据的vectot对象
         */
        template <typename T>
        static  void copyToCharVector ( std::vector<T>& srcData, int startIndex,  std::vector<char>& dstData);

        /**
         * @brief copyFromCharVector
         *             从char指针开始转换
         * @param srcData
         *              源数据的vector对象
         * @param startIndex
         *              拷贝源数据的起始索引
         * @param dstDataPtr
         *              目标数据的vectot对象
         */
        template <typename T>
        static void copyFromCharVector ( std::vector<char>& srcData,  int startIndex, std::vector<T>& dstDataPtr);

        //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    };//End of class DataConverter

    template<typename T>
    void  SSDK::DataConverter::copyToCharVector( std::vector<T>& srcData, int startIndex,  std::vector<char>& dstData)
    {
        if(srcData.size()  == 0)
        {
            //没有数据，直接退出
        }

        //起始索引不对，抛出异常
        if(srcData.size() <= startIndex)
        {
            std::ostringstream message;
            message<<" Start index[" << startIndex << "] is valid! ";
            THROW_EXCEPTION_WITHOUT_OBJ(message.str());
        }
        else
        {
            auto dstLen = srcData.size() * sizeof(T);
            if(dstLen!= dstData.size())
            {
                dstData.resize(dstLen);
            }

            auto  tmpCharPtr = reinterpret_cast<char*>(srcData.data() + startIndex);
            memcpy(dstData.data(), tmpCharPtr, dstLen);
        }
    }

    template <typename T>
    void  SSDK::DataConverter::copyFromCharVector (std::vector<char>& srcData,  int startIndex, std::vector<T>& dstDataPtr)
    {
        if(srcData.size()  == 0)
        {
            //没有数据，直接退出
        }

        //起始索引不对，抛出异常
        if(srcData.size() < startIndex + sizeof(T) )
        {
            std::ostringstream message;
            message<<" Start index[" << startIndex << "] is valid! ";
            THROW_EXCEPTION_WITHOUT_OBJ(message.str());
        }
        else
        {
            int dstLen = (srcData.size() - startIndex)/sizeof(T);

            if(dstDataPtr.size() != dstLen)
            {
                dstDataPtr.resize(dstLen);
            }

            auto tmpPtr = reinterpret_cast<T*>(srcData.data() + startIndex);
            memcpy(dstDataPtr.data(), tmpPtr, srcData.size() - startIndex);
        }
    }
}//End of namespace SSDK

#endif // DATACONVERT_HPP
