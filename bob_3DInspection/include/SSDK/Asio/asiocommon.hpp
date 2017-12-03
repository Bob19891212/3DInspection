#ifndef DATACONVERTER_HPP
#define DATACONVERTER_HPP

#include <vector>
#include <string>
#include <functional>


namespace SSDK
{
    namespace Asio
    {
        /**
         *  @brief AsioCommon
         *              Asio模块中调用的一些通用的函数
         *
         *  @author rime
         *  @version 1.00 2017-07-05 rime
         *                note:create it
         *
         * 定义在ASIO模块中公用的接口
         */
        class AsioCommon
        {
        public:

            //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //using

            /**
             *接受到数据调用的Handler的声明
             */
            using ReceivedHandler = std::function<void(
                const std::string& ip,//IP地址的string表示
                unsigned short port,//端口号
                const std::vector<char>& buf)>;//接受到的数据，存储在一个vector中

            /**
             *通讯发生错误调用的Handler声明
             */
            using RWErrorHandler = std::function<void(
                const std::string& ip,//IP地址的string表示
                unsigned short port,//端口号
                unsigned short connectID)>;//connection的ID

            //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

            //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //constructor & deconstructor

            AsioCommon();
            virtual ~AsioCommon();

            //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
          };//End of Class AsioCommon
    }//End of namespace Asio
}//End of namespace SSDK

#endif // DATACONVERTER_HPP
