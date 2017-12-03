#ifndef HANDLERINFO_H
#define HANDLERINFO_H

#include <string>

#include <QString>

#include "handlerinfo.hpp"
#include "metaeeum.hpp"

namespace SSDK
{
    namespace Exception
    {
        /**
         * it seems that these handler type are independence, so flag is no need
        */
        SEnum(HandlerType,
              AI_Response = 0,//detail hander function, it is designed by user
              Retry,//do again
              Ignore,//ignore and go on
              Abort,//stop and exit
    //                  Close
              );//exit,usually use in show msg only


        /**
         * Handler function Pointer,this struct(POD type) is used to write a text to represent a handle function
         */
        using HandlerFunc = void(*)(void);//param and return is all void

        /**
         * @brief The HandlerInfo class
         * Structure is used to handle exception and preserve releated message

         *  @author rime
         *  @version 1.00 2017-05-18 rime
         *                note:create it
         */
        class HandlerInfo
        {
        public:
            //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //constructor & deconstructor

            HandlerInfo(HandlerType handlerType,std::string msg,HandlerFunc func);
            HandlerInfo(HandlerType handlerType,std::string msg,QString displayMsg,HandlerFunc func);//QString是为了翻译
            HandlerInfo(HandlerType handlerType,QString displayMsg,HandlerFunc func);

            HandlerInfo();

            //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

            //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //get & set functions

            HandlerType handlerType() const{return m_handlerType;}

            const QString displayMsg()const{return this->m_displayMsg;}
            void setDisplayMsg(const QString& displayMsg){this->m_displayMsg = displayMsg;}

            //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

            void handler(){this->m_func();}

        private:

            HandlerType m_handlerType{HandlerType::Abort};

            //source option msg, it may be input in sdk,job or hw...(not app),it records original message
            std::string m_sourceMsg{""};

            //display exception msg, it is represented the translation of m_optionMsg,it is used in app project
            //because translation cannot cross projects(only apply in app project),so set m_displayMsg in App is
            //necessary.
            QString m_displayMsg{""};


            HandlerFunc m_func{nullptr};
        };//End of HandlerInfo

    }//End of namespace Exception
}//End of namespace SSDK

#endif // HANDLERINFO_H
