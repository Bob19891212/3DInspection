#ifndef FUNCTIONTRAITS_H
#define FUNCTIONTRAITS_H

#include <functional>


#ifdef __GNUC__
#include <cxxabi.h>
#endif

    namespace SSDK
    {
        namespace TypeTraits
        {
           /**
           *  @brief
           *        some reflection function related to "Function"
           *
           *  @author rime
           *  @version 1.00 2017-04-04 rime
           *                note:create it
           */
            class FunctionTraits
            {
            public:

                //This macro is used to check whether type has member function
                #define HAS_MEMBER(member)\
                template<typename T, typename... Args>\
                struct has_member_##member\
                {\
                private:\
                    template<typename U> \
                    static auto Check(int) -> \
                        decltype(std::declval<U>().member(std::declval<Args>()...),\
                        std::true_type()); \
                \
                    template<typename U> static std::false_type Check(...);\
                \
                public:\
                    enum{value = std::is_same<decltype(Check<T>(0)), std::true_type>::value};\
                };\

                //This macro is used to check whether function is member function
                #define IS_MEMBER()\
                struct has_member_##member\
                {\
                private:\
                    template<typename U> \
                    static auto Check(int) -> \
                        decltype(std::declval<U>().member(std::declval<Args>()...),\
                        std::true_type()); \
                \
                    template<typename U> static std::false_type Check(...);\
                \
                public:\
                    enum{value = std::is_same<decltype(Check<T>(0)), std::true_type>::value};\
                };
            };
        }//Core
    }//SSDK

#endif // FUNCTIONTRAITS_H
