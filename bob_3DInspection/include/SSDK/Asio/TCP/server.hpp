#ifndef TCPSERVER_HPP
#define TCPSERVER_HPP

#include <memory>
#include <vector>
#include <map>
#include <iostream>
#include <unordered_map>

#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/system/error_code.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/function.hpp>

#include <Exception/marcoexception.hpp>
#include <Asio/asiocommon.hpp>
#include <Asio/TCP/connection.hpp>
#include <Asio/TCP/rwhandler.hpp>

namespace SSDK
{
    namespace Asio
    {
        namespace TCP
        {
            /**
             *  @brief Server
             *
             *  @author rime
             *  @version 1.00 2017-07-05 rime
             *                note:create it
             *
             *  代表一个服务端，能够监听主机的一个端口
             */
            class Server
            {
            public:

                //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                //using

                /**
                 *对于一个包含了多个数据连接结构的Map
                 *      1.第一个参数：uint 表示连接的ID号，这个ID号不重复，范围为[1,65535], 对应了计算机的端口范围
                 *      2.第二个参数：std::shared_ptr<RWHandler>， 对应了每一个连接的读写数据结构， 由于通常Server和Client是不同机器的不同的进程，所以Server中只需要
                 *                                 对应的RWHandler即可
                 */
                using RWHandlerMap =  std::unordered_map<uint, std::shared_ptr<RWHandler>>;

                //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                //constructor & deconstructor

                /**
                 * @brief Server
                 * @param ios
                 *               Asio连接的Server对象，由外部创建
                 * @param port
                 *               监听的端口
                 * 注意：
                 *              1.对于服务端的总是在本机上运行，所以只需要制定端口即可，不需要IP地址，这个Connection还是不一样的
                 */
                Server(boost::asio::io_service & ios, uint port);
                virtual ~Server();

                //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                //get & set function

                /**
                 * @brief setReceivedHandler
                 *              设置接收到数据时触发的回调函数
                 * @param receivedHandler
                 *              接收数据触发的回调函数
                 */
                void setReceivedHandler( AsioCommon::ReceivedHandler receivedHandler )
                {
                    this->m_receivedCallBack = receivedHandler;
                }

                /**
                 * @brief connectionCnt
                 *             返回连接到当前服务器的数量
                 * @return
                 */
                uint connectionCnt(){ return  m_handlersMap.size();}

                //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                // conncet functions

                /**
                 * @brief createConnectionHandler
                 *              创建一个连接处理的Handler
                 * @return
                 *              包含了RWHandler的智能指针对象
                 *
                 * 注意：
                 *         1. 一开始监听的时候都会自动创建一个，后续每次有新的连接都会产生新的连接
                 *         2.智能指针不需要考虑到对象的销毁问题
                 */
                std::shared_ptr<RWHandler> createConnectionHandler();

                /**
                 * @brief accept
                 *              启动一个监听端口, 同时会调用一个异步的读取
                 */
                void accept();

                /**
                 * @brief releaseConnectID
                 *              释放Connection的ID，每次ID号释放后进入m_useableConnectionPool，可以给下一次是哟格
                 * @param connId
                 *              Connection的ID号
                 */
                void releaseConnectID(int connId);

                /**
                 * @brief copyDataFrom
                 *              从指定的ConnectionID读取数据
                 * @param connId
                 *              Connection的ID号
                 * @param dstData
                 *              由外部传入的用于接收数据的vector对象
                 */
                void copyDataFrom(int connId, std::vector<char>& dstData);

                /**
                 * @brief stop
                 *             停止监听
                 */
                void stop();

                 //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                //send functions

                /**
                 * @brief searchConnection
                 *             按照制定的IP和端口搜索连接（也就是客户端）
                 * @param ip
                 *             连接的IP地址，string类型
                 * @param port
                 *              连接的端口号，注意，这里是连接的端口号，而不是server的端口号，一个server对象只会对应一个固定的端口号
                 * @return
                 *              包含了RWHandler的智能指针对象
                 */
                std::shared_ptr<RWHandler> searchConnection(const std::string& ip, uint port);
                /**
                 * @brief searchConnection
                 *               按照制定的Connection的ID号搜索连接（也就是客户端）
                 * @param connID
                 *               Connection的ID号
                 * @return
                 *              包含了RWHandler的智能指针对象
                 */
                std::shared_ptr<RWHandler> searchConnection(uint connID);

                /*
                  *注意：
                  *          1.异步发送没有经过严格的测试，目前不建议使用
                  *          2.异步发送比同步发送更复杂，不稳定，所以优先使用同步发送
                  *          3.接受时涉及到同时有多个数据的接收，为了不阻塞，优先使用异步接收
                  */

                /**
                 * @brief sendAsync
                 *              异步发送数据
                 * @param ip
                 *              连接的IP地址， string类型
                 * @param port
                 *              连接的端口号
                 * @param sendedData
                 *              待发送的数据
                 */
                void sendAsync(const std::string& ip, uint port, const std::vector<char>& sendedData);

                /**
                 * @brief sendSync
                 *              同步发送数据
                 * @param ip
                 *              连接的IP地址， string类型
                 * @param port
                 *              连接的端口号
                 * @param sendedData
                 *              待发送的数据
                 */
                void sendSync(const std::string& ip, uint port, const std::vector<char>& sendedData);

                //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                //handle functions

                /**
                 * @brief handleAcceptError
                 *
                 * @param eventHanlder
                 * @param error
                 */
                void handleAcceptError(std::shared_ptr <RWHandler> eventHanlder, const boost::system::error_code& error);

                //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

            private:

               //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
               //private member variants

                /**
                 * @brief m_ioService
                 *            io_service实例，Boost.Asio使用io_service同操作系统的输入/输出服务进行交互
                 */
                boost::asio::io_service& m_ioService;

                /**
                 * @brief m_acceptor
                 *        接受对象
                 */
                boost::asio::ip::tcp::acceptor m_acceptor;

                /**
                  * @brief m_handlersMap
                  *             读写处理的Map
                  */
                 RWHandlerMap m_handlersMap;

                 /**
                  * @brief m_useableConnIDPool
                  *             可用的连接ID池，每次连接结束后，ID会释放到这个池子中
                  */
                 std:: list<int> m_useableConnectionPool;

                 /**
                  * @brief m_receivedHandler
                  *             接受数据执行的回调函数
                  */
                 AsioCommon::ReceivedHandler m_receivedCallBack { nullptr } ;

               //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

            };//End of Server
        }//End of namespace TCP
    }//End of namespace Asio
}//End of namespace SSDK

#endif // TCPSERVER_HPP
