#ifndef SEXCEPTION_H
#define SEXCEPTION_H

#include <exception>
#include <sstream>
#include <typeinfo>
#include <bitset>
#include <memory>
#include <iostream>

#include <QObject>
#include <QString>

#include "idisposal.hpp"
#include "TypeTraits/functionaltraits.hpp"
#include "TypeTraits/typetraits.hpp"
#include "metaeeum.hpp"
#include "handlerinfo.hpp"
#include "marcoexception.hpp"

namespace SSDK
{
        namespace Exception
        {
        /*use map, although can put HandlerType to HandlerInfo, but map can use this form as obj[HandlerType::Retry],
        it is called by higher project conveniently
        */
        using ExceptionHandlers = std::map<SSDK::Exception::HandlerType,SSDK::Exception::HandlerInfo>;


        /**
         *  @brief  Derived class inherited std::exception.
         *
         *  it constains
         *      1.Disposal Pointer, can be passed to higher module
         *      2.More rich Message describing cause of error, it can be modified and appended
         *
         *  @author rime
         *  @version 1.00 2017-04-04 rime
         *                note:create it
         */
        class CustomException:public std::exception
        {
        public:

            //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //constructors & deconstructor

            CustomException();

            /**
             * 注意，这里的pDisposal一定不能是const，因为会调用IDisposal的dispose函数，
             *而dispose涉及到销毁内部的数据，所以一定不是const的，这在创建异常的时候
             *需要特别注意.
             */
            CustomException(std::string& message, IDisposal* pDisposal);
            CustomException(const CustomException& customException);
            CustomException( CustomException&& customException);

            virtual~ CustomException();

            //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

            //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //operator functions

            //lvalue assignment operator function
            CustomException& operator=(const CustomException& customException);
            //rvalue assignment operator function
            CustomException& operator=(CustomException&& customException);

            //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

            //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //get & set functions

            //This function is used to set m_displayMsg
            void setDisplayMsg(const QString& displayMsg){this->m_displayMsg = displayMsg;}
            //This function is used to get m_displayMsg
            const QString displayMsg()const{return this->m_displayMsg;}

            //This function is used to set m_msgShownMode
            void setStackTraceShown(bool stackTraceShownMode)
            {
                this->m_isStackTraceMsgShown = stackTraceShownMode;
            }

            //This function is used to get m_msgShownMode
            bool isStackTraceMsgShown()
           {
               return this->m_isStackTraceMsgShown ;
           }

            //This function is used to get exceptionMsg,
            //    if m_isStackTraceShown is true ,exceptionMsg is m_stackTraceMsg
            //    if m_isStackTraceShown is false,exceptionMsg is m_orgMsg
            std::string getExceptionMsg() const;

            //This function is used to get m_orginalMsg. it is a read-only variant
            const std::string orginalMsg() const{return this->m_orginalMsg;}

            //This function is used to get m_stackTraceMsg
            const std::string stackTraceMsg()const{return this->m_stackTraceMsg;}
            //This function is used to set m_stackTraceMsg
            void setStackTraceMsg(const std::string& stackTraceMsg)
            {
                this->m_stackTraceMsg  = stackTraceMsg;
            }

            //This function is used to set dispaly message based on HandlerType for handlers
            void setHandlerDisplayMsg(HandlerType handlerType,const QString& displayMsg)
            {
                auto iterator = this->m_handlers.find(handlerType);
                if(this->m_handlers.end() == iterator)
                    return;

                this->m_handlers[handlerType].setDisplayMsg(displayMsg);
            }

            //This function is used to set m_handlers
            void setHandlers( ExceptionHandlers handlers);
            //This function is used to get m_handlers
            ExceptionHandlers handlers()const{return this->m_handlers;}

            //This function is used to get m_pDisposal
            const IDisposal* iDisposal()const{return this->m_pDisposal;}

            //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

            //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //normal functions

            //This function is used to get message, overrided exception's what() function
            /** Returns a C-style character string describing the general cause
             *  of the current error.  */
            virtual const char* what() const _GLIBCXX_USE_NOEXCEPT override
            {
                return m_stackTraceMsg.data();
            }

            //This function is used to get  handler name list
            const std::list<std::string> getHandlerTypeNameList()const;

            //This function is used to dispose resource, it depends on m_pDisposal, if m_pDisposal is null,do nothing
            void dispose()
            {
                if(nullptr!=m_pDisposal)
                    this->m_pDisposal->dispose();
            }

            //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        private:

            //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //member variant

            // This is string of display message,it can be translated by user for UI,Default is empty
            //because translation cannot cross projects(only apply in app project),so set m_displayMsg in App is
            //necessary.
            QString m_displayMsg{""};

            // This is string of orginal exception message.
            std::string m_orginalMsg{""};

            // This is string of the whole stack trace.
            std::string m_stackTraceMsg{""};

            //This variant holds is whether show stack trace exception message
            bool m_isStackTraceMsgShown{false};

            //This variant represented interface which can dispose resource
            IDisposal*  m_pDisposal{nullptr};

            //This is exception handler map.
            ExceptionHandlers m_handlers;

            //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        };//End of CustomException
        }//End of  Exception
    }//End of SSDK

#endif // SEXCEPTION_H
