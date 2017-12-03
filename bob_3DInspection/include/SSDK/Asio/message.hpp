#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <cstddef>
#include <cstring>
#include <vector>
#include <Asio/asiocommon.hpp>

namespace SSDK
{
    namespace Asio
    {
            /**
             *  @brief Message
             *
             *  @author rime
             *  @version 1.00 2017-07-07 rime
             *                note:create it
             */
            class Message
            {
            public:

                //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                //enum

                /**
                 * 数据长度值的最大位数，2的8次方即64位整数值区间为[0, 18,446,744,073,709,551,615 ]
                 */
                enum{ HEADER_LENGTH = 8 };

                //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                //constructor & deconstructor

                /**
                 * @brief Message
                 *             默认构造函数，不带任何参数
                 */
                Message();
                /**
                 * @brief Message
                 * @param pData
                 *              char指针，当输入为null时，即输入一个空值
                 * @param len
                 *              指针的长度，当输入为0，即输入一个空值
                 */
                Message(const  char* pData, uint len);
                /**
                 * @brief Message
                 * @param data
                 *              创建的数据
                 */
                Message(const std::vector<char>& data);

                ~Message();

                //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                //get & set functions

                /**
                 * @brief pHeader
                 *             头指针，也是缓存数据的头指针
                 * @return
                 */
                char* pHeader(){ return this->m_buff.data(); }
                /**
                 * @brief pBody
                 *             数据包指针，缓存了数据
                 * @return
                 */
                char* pBody(){ return this->pHeader() + HEADER_LENGTH;}
                /**
                 * @brief buff
                 * @return
                 *              返回Message的Vector数据结构
                 */
                std::vector< char>& buff(){ return this->m_buff;}

                /**
                 * @brief headerLength
                 * @return
                 *             头文件的长度
                 */
                std::size_t headerLength() const { return HEADER_LENGTH;}
                /**
                 * @brief bodyLength
                 * @return
                 *              数据的长度
                 */
                std::size_t bodyLength() const { return this->m_bodyLength; }
                /**
                 * @brief messageLength
                 * @return
                 *             Message的长度
                 */
                std::size_t messageLength()  { return this->m_buff.size(); }

                //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                //write functions

                /**
                  * @brief writeHeader
                  *             写数据长度到数据包中
                  * @param len
                  *             数据长度
                  */
                 void writeHeader( uint len);

                 /**
                      * @brief writeData
                      *             写数据到数据包
                      * @param pData
                      *             数据的头指针
                      * @param len
                      *             数据长度
                      */
                 void writeData( const  char* pData, uint len);
                 /**
                  * @brief writeData
                  *             写数据到数据包
                  * @param data
                  *             需要写入的数据，存放在一个vector中
                  */
                 void writeData( const std::vector<char>& data);

                 /**
                  * @brief clearData
                  *              清空数据
                  */
                 void clearData();

                //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                 //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                 //auxiliary functions

                 /**
                  * @brief parseHeader
                  *             解析出数据包中数据的长度
                  * @param pHeader
                  *             Message的头指针
                  * @return
                  *             数据包的长度
                  */
                 uint64_t static parseHeader(char* pHeader);

                 //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

            private:

                /**
                 * @brief m_bodyLength
                 *          实际数据包的长度
                 */
                std::size_t m_bodyLength {0};

                /**
                 * @brief m_buff
                 *          缓存的数据
                 */
                std::vector< char> m_buff ;

            };//End of class Message
    }//End of namespace Asio
}//End of namespace SSDK

#endif // MESSAGE_HPP
