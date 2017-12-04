#ifndef MARCOEXCEPTION_H
#define MARCOEXCEPTION_H

#include "customexception.hpp"

namespace SSDK
{
    namespace Exception
    {
        /**
         *  @brief
         *      Macros related to throw and handle excpetion
         *
         * 1.THROW_EXCEPTION_WITH_OBJ:
         *    THROW_EXCEPTION_WITHOUT_OBJ
         *
         *      used in the initialied location of the exception(as SDK), throw exception with stacktrace message and no handler,
         * usually only show stack trace message
         *
         *
         * 2.CATCH_THROW_EXCEPTION_WITH_OBJ
         *    CATCH_THROW_EXCEPTION_WITHOUT_OBJ
         *
         *      used in throw excepction in the catch '{}',it appended extral message to exception and create customException instance
         *
         *
         * 3.CATCH_AND_RETHROW_EXCEPTION
         *      RETHROW_EXCEPTION_WITH_OBJ
         *      APPEND_EXCEPTION_MSG_WITH_OBJ
         *      APPEND_EXCEPTION_MSG_WITH_OBJ
         *
         *    CATCH_AND_RETHROW_EXCEPTION_WITH_OBJ
         *       RETHROW_EXCEPTION_WITHOUT_OBJ
         *      APPEND_EXCEPTION_MSG_WITHOUT_OBJ
         *      APPEND_EXCEPTION_MSG_WITHOUT_OBJ
         *
         *      used in rethrow exception,is not the original location of the exception(as Job:exception is caused by another called function),
         * it append stack trace message to original excepional message
         *
         *
         * 4.THROW_EXCEPTION_WITH_HANDLERS_WITH_OBJ
         *
         *      used in some occasion which need some complicated handler, these handlers are too cautious and important(as datastruct and
         * hardware), so these handler should be coded by special people, not everyone can do.
         *
         * Rest of other macros cooperated to these 4 occasions
         *
         * Note:
         *      1. 由于宏定义仅仅是复制粘贴，所以缺乏编译的类型检查，所有在使用下面宏定义记得不能加namespace，否则回编译不过
         *      如：
         *          THROW_EXCEPTION("111", this)                                      正确
         *          SSDK::Exception::THROW_EXCEPTION("111", this)       错误
         *
         *  @author rime
         *  @version 1.00 2017-04-03 rime
         *                note:create it
         */

    /** 注意:
      *     1.throw和catch只有通过宏定义才有意义, 利用了宏定义等于是粘贴代码的特性, 可以准确捕捉到异常处的类/函数等信息,
      *         否则如果利用封装的函数的话, 最终异常的起点只会指向异常的函数
      *     2.因为宏定义中if else等条件语句是无法按照运行时正常编译的, 同时宏函数又不支持重载, 所以所有的宏函数分成WithObj
      *        和withoutObj两个版本. 前者用于根据Obj显示出类型, 如果是IDisposal的话,还支持dispose函数; 后者用于像static函数这种
      *         无法获取到对象指针的场合
      */

        #define THROW_EXCEPTION_WITH_OBJ(exMsg)\
        {\
            std::ostringstream message;\
            message<<"<p><b>File&nbsp;&nbsp;:&nbsp;&nbsp;</b>"<<__FILE__<<"</p>"\
                           <<"<p><b>Line&nbsp;&nbsp;:&nbsp;&nbsp;</b>"<<__LINE__<<"</p>"\
                           <<"<p><b>Class&nbsp;&nbsp;:&nbsp;&nbsp;</b>"<<SSDK::TypeTraits::TypeTraits::typeName(this).get()<<"</p>"\
                           <<"<p><b>Func&nbsp;&nbsp;:&nbsp;&nbsp;</b>"<<__FUNCTION__<<"</p>"\
                           <<"<p><b>Detail&nbsp;&nbsp;:&nbsp;&nbsp;</b>"<<exMsg<<"</p>";\
             std::string msg = message.str();\
                    \
             if(typeid(this) == typeid(SSDK::Exception::IDisposal) )\
             {\
                       throw  SSDK::Exception::CustomException(\
                            msg,\
                            const_cast< SSDK::Exception::IDisposal*> (reinterpret_cast<const SSDK::Exception::IDisposal*>(this)));\
              }\
              else\
              {\
                        throw  SSDK::Exception::CustomException(msg,nullptr);\
              }\
        }
        #define THROW_EXCEPTION_WITHOUT_OBJ(exMsg)\
        {\
            std::ostringstream message;\
            message<<"<p><b>File&nbsp;&nbsp;:&nbsp;&nbsp;</b>"<<__FILE__<<"</p>"\
                <<"<p><b>Line&nbsp;&nbsp;:&nbsp;&nbsp;</b>"<<__LINE__<<"</p>"\
                <<"<p><b>Func&nbsp;&nbsp;:&nbsp;&nbsp;</b>"<<__FUNCTION__<<"</p>"\
                <<"<p><b>&nbsp;&nbsp;:&nbsp;&nbsp;</b>"<<exMsg<<"</p>";\
            std::string msg = message.str();\
            throw  SSDK::Exception::CustomException(msg,nullptr);\
        }\

        //Occasion2: This macro is used to throw excepction in the catch '{}', it appended extral message to exception and create
        //CustomException instance
        #define CATCH_THROW_EXCEPTION_WITH_OBJ(exMsg)\
        catch (...)\
        {\
            THROW_EXCEPTION_WITH_OBJ(exMsg);\
        }
        #define CATCH_THROW_EXCEPTION_WITHOUT_OBJ(exMsg)\
        catch (...)\
        {\
            THROW_EXCEPTION_WITHOUT_OBJ(exMsg);\
        }

        //Occasion3: This macro is used to catch and rethrow excepction by middle level
        #define CATCH_AND_RETHROW_EXCEPTION_WITH_OBJ(appendedMsg)\
        catch (SSDK::Exception::CustomException& ex )\
        {\
            RETHROW_EXCEPTION_WITH_OBJ(ex,appendedMsg)\
        }
        #define CATCH_AND_RETHROW_EXCEPTION_WITHOUT_OBJ(appendedMsg)\
        catch (SSDK::Exception::CustomException& ex )\
        {\
            RETHROW_EXCEPTION_WITHOUT_OBJ(ex,appendedMsg)\
        }

        //Occasion4: This macro is used to throw excepction with handlers(as Hw)
        #define THROW_EXCEPTION_WITH_HANDLERS_WITH_OBJ(exMsg,handlers)\
        {\
                std::ostringstream message;\
                message<<"<p><b>File&nbsp;&nbsp;:&nbsp;&nbsp;</b>"<<__FILE__<<"</p>"\
                       <<"<p><b>Line&nbsp;&nbsp;:&nbsp;&nbsp;</b>"<<__LINE__<<"</p>"\
                       <<"<p><b>Class&nbsp;&nbsp;:&nbsp;&nbsp;</b>"<<SSDK::TypeTraits::TypeTraits::typeName(this).get()<<"</p>"\
                       <<"<p><b>Func&nbsp;&nbsp;:&nbsp;&nbsp;</b>"<<__FUNCTION__<<"</p>"\
                       <<"<p><b>Detail&nbsp;&nbsp;:&nbsp;&nbsp;</b>"<<exMsg<<"</p>";\
                std::string msg = message.str();\
                \
                if(std::is_base_of<IDisposal,std::remove_pointer<decltype(this)>::type>::value)\
                {\
                    CustomException ex(msg,reinterpret_cast<IDisposal*>(this));\
                    ex.setHandlers(handlers);\
                    throw ex;\
                }\
                else\
                {\
                    CustomException ex(msg,nullptr);\
                    ex.setHandlers(handlers);\
                    throw ex;\
                }\
        }
        #define THROW_EXCEPTION_WITH_HANDLERS_WITHOUT_OBJ(exMsg,handlers)\
        {\
                std::ostringstream message;\
                message<<"<p><b>File&nbsp;&nbsp;:&nbsp;&nbsp;</b>"<<__FILE__<<"</p>"\
                       <<"<p><b>Line&nbsp;&nbsp;:&nbsp;&nbsp;</b>"<<__LINE__<<"</p>"\
                       <<"<p><b>Func&nbsp;&nbsp;:&nbsp;&nbsp;</b>"<<__FUNCTION__<<"</p>"\
                       <<"<p><b>Detail&nbsp;&nbsp;:&nbsp;&nbsp;</b>"<<exMsg<<"</p>";\
                std::string msg = message.str();\
                \
                CustomException ex(msg,nullptr);\
                ex.setHandlers(handlers);\
                throw ex;\
        }

        //This macro is used to rethrow excepction(as Job)
        #define RETHROW_EXCEPTION_WITH_OBJ(ex,appendedMsg)\
        {\
            APPEND_EXCEPTION_MSG_WITH_OBJ(ex,appendedMsg);\
            throw ex;\
        }
        #define RETHROW_EXCEPTION_WITHOUT_OBJ(ex,appendedMsg)\
        {\
            APPEND_EXCEPTION_MSG_WITHOUT_OBJ(ex,appendedMsg);\
            throw ex;\
        }

        //This macro is used to append excepction message and rethrow exception in middle level
        #define APPEND_EXCEPTION_MSG_WITH_OBJ(ex,appendedMsg)\
        {\
           std::ostringstream message;\
           message<<"<p><b>File&nbsp;&nbsp;:&nbsp;&nbsp;</b>"<<__FILE__<<"</p>"\
                         <<"<p><b>Line&nbsp;&nbsp;:&nbsp;&nbsp;</b>"<<__LINE__<<"</p>"\
                         <<"<p><b>Class&nbsp;&nbsp;:&nbsp;&nbsp;</b>"<<SSDK::TypeTraits::TypeTraits::typeName(this).get()<<"</p>"\
                         <<"<p><b>Func&nbsp;&nbsp;:&nbsp;&nbsp;</b>"<<__FUNCTION__<<"</p>"\
                           <<"<p></p>"<< ex.stackTraceMsg();\
            std::string msg = message.str();\
            ex.setStackTraceMsg(msg);\
            throw ex ;\
        }
        #define APPEND_EXCEPTION_MSG_WITHOUT_OBJ(ex,appendedMsg)\
        {\
           std::ostringstream message;\
           message<<"<p><b>File&nbsp;&nbsp;:&nbsp;&nbsp;</b>"<<__FILE__<<"</p>"\
                         <<"<p><b>Line&nbsp;&nbsp;:&nbsp;&nbsp;</b>"<<__LINE__<<"</p>"\
                         <<"<p><b>Func&nbsp;&nbsp;:&nbsp;&nbsp;</b>"<<__FUNCTION__<<"</p>"\
                           <<"<p></p>"<< ex.stackTraceMsg();\
            std::string msg = message.str();\
            ex.setStackTraceMsg(msg);\
            throw ex ;\
        }

    }//End of namespace Exception
}//End of namespace SSDK

#endif // MARCOEXCEPTION_H
