#ifndef RWHANDLER_HPP
#define RWHANDLER_HPP

#include <functional>

#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/system/error_code.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>

#include <Exception/marcoexception.hpp>
#include <Asio/message.hpp>
#include <Asio/asiocommon.hpp>

namespace SSDK
{
    namespace Asio
    {
        namespace TCP
        {
            /**
             *  @brief RWHandler
             *
             *  @author rime
             *  @version 1.00 2017-07-07 rime
             *                note:create it
             *
             * 注意：
             *          1.需要通过enable_shared_from_this返回this指针，这样才能保证异步操作时，原来的生命周期不会结束
             *             在返回时还是有效的
             *
             *          2.异步发送没有经过严格的测试，目前不建议使用， 除此之外，还是不推荐使用异步发送
             *
             *             异步发送比异步接收等其他异步操作更复杂，需要一个发送队列来保证发送不会乱序.
             *             但是，还有一个问题需要注意就是这个发送队列是没有加限制的，如果接收端收到数据之后阻塞处理，
             *             而发送又很快的话，就会导致发送队列的内存快速增长甚至内存爆掉。解决办法有两个：
             *                      1.发慢一点，并且保证接收端不会长时间阻塞socket；
             *                      2.控制发送队列的上限。
             *              第一种方法对实际应用的约束性较强，实际可操作性不高。
             *              第二种方法需要控制队列上限，不可避免的要加锁，这样就丧失了单线程异步发送的性能优势。
             *
             *              所以建议用同步发送接口来发送数据：
             *                      一来不用发送队列，自然也不会有内存暴涨的问题，
             *                      二来也不会有复杂的循环发送过程，而且还可以通过线程池来提高发送效率。
             *
             *          总结
             *              1.不要连续发起异步发送，要等上次发送完成之后再发起下一个异步发送；
             *              2.要考虑异步发送的发送队列内存可能会暴涨的问题；
             *              3.相比复杂的异步发送，同步发送简单可靠，推荐优先使用同步发送接口。
             *
             *          异步发送没有经过严格的测试，目前不建议使用
             *
             *          3.RWHandler结构保存了客户端和服务端通用的读写接口，所以在客户端和服务端都可以调用RWHandler，
             *             它抽象了公共的读写行为
             */
            class RWHandler: public std::enable_shared_from_this<RWHandler>
            {
            public:

                //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                //constructor & deconstructor

                /**
                 * @brief RWHandler
                 * @param ios
                 *              boost::asio的io_service对象，注意，这里传入的参数是引用，说明了该对象是由外部创建
                 *      不需要在内部负责销毁
                 */
                RWHandler(boost::asio::io_service& ios);
                virtual ~RWHandler();

                //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                //get & set function

                /**
                 * @brief socket
                 *          连接的套接字
                 * @return
                 */
                boost::asio::ip::tcp::socket& socket() {return this->m_socket;}
                /**
                 * @brief ip
                 *          返回连接点的IP
                 * @return
                 */
                const std::string ip(){return this->m_socket.remote_endpoint().address().to_string();}
                /**
                 * @brief port
                 *           返回连接点的端口号
                 * @return
                 */
                unsigned short port(){return this->m_socket.remote_endpoint().port();}

                /**
                 * @brief setId
                 *          设置连接的ID号，范围取决与Server的最大连接数
                 * @param id
                 */
                void setId(uint id){this->m_Id = id;}
                /**
                 * @brief Id
                 *          返回当前连接的ID号
                 * @return
                 */
                uint Id() const {return this->m_Id;}

                /**
                 * @brief receivedMessage
                 *             返回接受到Message对象，数据存储在里面
                 * @return
                 */
                 Message& receivedMessage(){return this->m_receivedMessage;}

                /**
                 * @brief setCallBackError
                 *             设置读写错误是的回调函数
                 * @param errorHandler
                 *              读写错误时的回调函数
                 */
                void setErrorHandler(AsioCommon::RWErrorHandler errorHandler){ this->m_errorHandler = errorHandler; }

                /**
                 * @brief setReceivedHandler
                 *             设置接受到数据的处理Handler
                 * @param receivedHandler
                 *              数据接受到的回调函数
                 */
                void setReceivedHandler(AsioCommon::ReceivedHandler receivedHandler){ this->m_receivedHandler =  receivedHandler;}

                //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                //send & receive function

               /*
                 *注意：
                 *          1.异步发送没有经过严格的测试，目前不建议使用
                 *          2.异步发送比同步发送更复杂，不稳定，所以优先使用同步发送
                 *          3.接受时涉及到同时有多个数据的接收，为了不阻塞，优先使用异步接收
                 */

                /**
                 * @brief sendSync
                 *             同步发送
                 * @param pData
                 *             需要发送的数据指针
                 * @param len
                 *             需要发送的数据对象
                 */
                void sendSync(const  char* pData, int64_t len);
                void sendSync(const std::vector<char>& toSendedData);

                /**
                 * @brief receiveSync
                 *             同步接受
                 */
                void receiveSync();
                /**
                 * @brief receiveAsync
                 *              异步接收
                 */
                void receiveAsync();

                void sendAsync(const std:: vector<char>& toSendedData);
                /**
                 * @brief sendAsync
                 *             异步发送数据
                 * @param pData
                 *              数据对象指针
                 * @param len
                 *              发送的数据长度
                 */
                void sendAsync(const  char* pData, int64_t len);
                void sendAsync();

                /**
                 * @brief close
                 *             关闭当前的RWHandler对象
                 */
                void close();

                //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                //handle function

                void handleError(const boost::system::error_code& error);

                //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

            private:

                //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                //member variant

                /**
                 * @brief m_socket
                 *          连接的套接字对象
                 */
                boost::asio::ip::tcp::socket m_socket;

                /**
                 * @brief m_Id
                 *          连接的ID号
                 */
                uint m_Id;

                /**
                 * @brief m_callbackError
                 *             错误的回调函数
                 */
                AsioCommon::RWErrorHandler m_errorHandler;

                /**
                 * @brief m_receivedHandler
                 *             获取数据后执行的回调函数, 通常用于上层调用的数据处理接口
                 */
                AsioCommon::ReceivedHandler m_receivedHandler;

                /**
                 * @brief m_sendedMessage
                 *          这个Message对应接受到的信息，目前只支持char类型，封装在Message中，在进行通讯之间需要用DataConveter进行类型装换
                 */
                Message m_receivedMessage;

                /**
                 * @brief m_toSendedMessage
                 *             有待发送的Message，目前只支持char类型，封装在Message中，在进行通讯之间需要用DataConveter进行类型装换
                 */
                Message m_toSendedMessage;


                /**
                 * @brief m_toSendMessageQueue
                 *          有待发送的消息队列
                 * 异步发送的复杂性在于：
                 *
                 *          不能连续调用异步发送接口async_write，因为async_write内部是不断调用async_write_some，
                 * 直到所有的数据发送完成为止.
                 *
                 *          有可能这时第一次的async_write还在循环调用async_write_some发送，而第二次的async_write要发送的数据很小，
                 * 一下子就发出去了，这使得第一次发送的数据和第二次发送的数据交织在一起了，导致发送乱序的问题.
                 *
                 *          这个问题的方法就是在第一次发送完成之后再发送第二次的数据。具体的做法是用一个发送缓冲区，
                 * 在异步发送完成之后从缓冲区再取下一个数据包发送
                 */
                std::list<Message> m_toSendedMessageQueue;

                //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            };//End of class RWHandler
        }//End of namespace TCP
    }//End of namespace Asio
}//End of namespace SSDK

#endif // RWHANDLER_HPP
