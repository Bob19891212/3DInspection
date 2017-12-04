#ifndef INONCOPYABLE_H
#define INONCOPYABLE_H

#include "inonassignable.hpp"

namespace SSDK
{
        /**
         *  @brief
         *      forbid copy, it contains
         *            1.forbid assignment, so inhert INonAssignable
         *            2.forbid copy, so copy constructor is private
         *
         *      non-copyanble class need inhert it
         *
         *  @author rime
         *  @version 1.00 2017-03-02 rime
         *                note:create it
        */
        class INonCopyable:private INonAssignable
        {
        protected:
            INonCopyable(){}
            virtual ~INonCopyable(){}

        private:
            INonCopyable(const INonCopyable&);
        };
}//End of namespace SSDK

#endif//INONCOPYABLE_H
