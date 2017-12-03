#ifndef TCPCLIENT_HPP
#define TCPCLIENT_HPP

#include <memory>
#include <vector>
#include <deque>
#include <iostream>
#include <thread>
#include <functional>

#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/system/error_code.hpp>
#include <boost/bind.hpp>
#include <boost/thread/pthread/condition_variable.hpp>

#include <inoncopyable.hpp>
#include <Exception/marcoexception.hpp>
#include <Asio/asiocommon.hpp>
#include <Asio/TCP/rwhandler.hpp>

namespace SSDK
{
    namespace Asio
    {
        namespace TCP
        {
            /**
             *  @brief TCP
             *
             *  @author rime
             *  @version 1.00 2017-07-05 rime
             *                note:create it
             *
             * 代表了一个远程的连接，即客户端，
             * 一个客户端同时只能连接一个服务端，但是一个服务端能够同时连接多个客户端
             * 需要制定服务器的IP和端口号
             */
            class Connection
            {
            public:

                //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                //constructor & deconstructor

                /**
                 * @brief Connection
                 * @param ioService
                 *                Asio连接的Server对象，由外部创建
                 * @param remoteIP
                 *                远程连接的IP地址
                 * @param remotePort
                 *                远程连接的端口号
                 */
                Connection(boost::asio::io_service& ioService,
                           const std::string& remoteIP,
                           uint remotePort);
                ~Connection();

                //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                //get & set fuunction

                /**
                 * @brief remoteIP
                 *             远程服务器的IP号
                 * @return
                 */
                std::string remoteIP(){  return this->m_serverEndPoint.address().to_string();}
                /**
                 * @brief serverPort
                 *             服务器的端口号
                 * @return
                 */
                uint remotePort(){ return this->m_serverEndPoint.port();}

                /**
                 * @brief setReceivedHandler
                 *             设置接收到数据时触发的回调函数
                 * @param receivedHandler
                 *              接收数据触发的回调函数
                 */
                void setReceivedHandler( AsioCommon::ReceivedHandler receivedHandler ){this->m_eventHandler->setReceivedHandler(receivedHandler);}
                /**
                 * @brief setErrorHandler
                 *             设置发生错误时触发的回调函数
                 * @param errorHandler
                 *              发生错误时触发的回调函数
                 */
                void setErrorHandler( AsioCommon::RWErrorHandler errorHandler){ this->m_eventHandler->setErrorHandler(errorHandler); }

                /**
                 * @brief isConnected
                 * @return 是否处于连接状态
                 */
                bool isConnected() const{return this->m_isConnected;}

                //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                //send functions

                /*
                  *注意：
                  *          1.异步发送没有经过严格的测试，目前不建议使用
                  *          2.异步发送比同步发送更复杂，不稳定，所以优先使用同步发送
                  *          3.接受时涉及到同时有多个数据的接收，为了不阻塞，优先使用异步接收
                  */

                /**
                 * @brief sendSync
                 *             同步发送数据
                 * @param sendedData
                 *             待发送的数据
                 *
                 * 注意：
                 *          发送数据有同步发送和异步发送，对于绝大部分情况，同步发送已经足够; 异步发送复杂不可靠
                 */
                void sendSync(std::vector<char>& sendedData);
                /**
                 * @brief sendAsync
                 *             异步发送数据
                 * @param sendedData
                 *             待发送的数据
                 *
                 * 注意：
                 *        发送数据有同步发送和异步发送，对于绝大部分情况，同步发送已经足够;  异步发送复杂不可靠
                 */
                void sendAsync(std::vector<char>& sendedData);

                //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                //receive functions

                /**
                 * @brief receiveAsync
                 *             异步接受数据
                 *
                 * 注意：
                 *          和发送数据相反，因为大部分场景都是一个服务端同时接受多个端口的数据，所以异步更好
                 */
                void receiveAsync();
                /**
                 * @brief receiveSync
                 *            同步接受数据
                 */
                void receiveSync();

                //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                //connect functions

                /**
                 * @brief connect
                 *          连接到服务器
                 * @return
                 *          连接成功，返回true，否则为false
                 */
                bool connect();
                /**
                 * @brief reconnect
                 *          重新连接， 这里会开辟新的线程进行重连
                 */
                void reconnect();
                /**
                 * @brief setReconnectParm
                 *             设置重新连接的选项参数
                 * @param isRepeatConnect
                 *              是否需要重新连接
                 * @param maxRepeatConnectCnt
                 *              最大的连接次数
                 * @param connectIntervalMS
                 *              连接的间隔，单位ms
                 */
                void setReconnectParm(
                        bool isRepeatConnect,
                        uint maxRepeatConnectCnt,
                        uint connectIntervalMS);
                /**
                 * @brief disconnect
                 *      断开连接
                 */
                void disconnect();

                //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

            private:

                //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                //handler functions

                /**
                 * @brief createRWHandler
                 *             创建一个读写的Handler
                 * @param ioService
                 */
                void createRWHandler(boost::asio::io_service& ioService);
                /**
                 * @brief handleRWError
                 *             处理读写错误
                 * @param connid
                 *              连接的ID号
                 */
                void handleError(std::string ip, unsigned short port,  unsigned short connID);
                /**
                 * @brief handleConnectError
                 *             处理连接错误
                 * @param error
                 */
                void handleConnectError(const boost::system::error_code& error);

                //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                //private member variant

                /**
                  * @brief m_ioservice
                  *             boost.Asio使用io_service同操作系统的输入/输出服务进行交互
                  */
                 boost::asio:: io_service& m_ioservice;
                 /**
                  * @brief m_socket
                  *             套接字对象
                  */
                 boost::asio::ip::tcp::socket m_socket;

                 /**
                  * @brief m_serverEndPoint
                  *             服务器的IP和端口信息
                  */
                 boost::asio::ip:: tcp::endpoint m_serverEndPoint;

                 /**
                  * @brief m_eventHandler
                  *             该连接对应的读写Handler
                  */
                 std::shared_ptr<RWHandler> m_eventHandler;

                 /**
                    * @brief m_isConnected
                    *      是否已经打开连接
                    */
                 bool m_isConnected{false};
                 /**
                  * @brief m_reconnectingThread
                  *             用于连接失败的时候重新连接的线程， 因为在一个线程中只能进行一个异步的连接， 所以
                  *     重连必须重新开辟线程
                  */
                 std::shared_ptr<std::thread> m_reconnectingThread{nullptr};
                 /**
                  * @brief m_isRepeatConnect
                  *             如果没有连接上，是否需要重新连接，默认为true
                  */
                 bool m_isReconnectRequired { true };
                 /**
                  * @brief m_maxRepearConnectCnt
                  *             最大的重新连接次数
                  */
                 uint m_maxReconnectCnt { 50};
                 /**
                  * @brief m_currentRepeatConnectIndex
                  *         当前重新连接的次数
                  */
                 uint m_currentReconnectIndex {0};
                 /**
                  * @brief m_connectIntervalTime
                  *         重新连接的时间间隔，单位ms
                  */
                 uint m_connectIntervalTime{1000};
                 /**
                  * @brief m_isReconnecting
                  *            是否处于重连接中
                  */
                 bool m_isReconnecting {false};

                //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

            };//End of class Connection
        }//End of namespace TCP
    }//End of namespace Asio
}//End of namespace SSDK

#endif // TCPCLIENT_HPP
