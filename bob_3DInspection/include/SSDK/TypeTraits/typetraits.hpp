#ifndef TYPETRAITS_H
#define TYPETRAITS_H

#include <memory>
#include <string>
#include <typeinfo>
#include <cxxabi.h>

#include <QString>

     namespace SSDK
    {
        namespace TypeTraits
        {
            /**
            *  @brief
            *       get string of Type
            *       eg:
            *           Student(type:class) -> "Student"(type:string)
            *
            *  @param typeName: name of Type
            *           eg: TYPE_STRING_NAME(Student)
            *  @return type name string
            *           eg: TYPE_STRING_NAME(Student) return "Student"
            */
            #define TYPE_STRING_NAME( typeName ) # typeName

            /**
            *  @brief
            *       get string of type, it contains complete signature information
            *       eg:
            *           "Sung::SSDK::Core::TypeTraits"
            *
            *  @author rime
            *  @version 1.00 2017-04-04 rime
            *                note:create it
            */
            class TypeTraits
            {
            public:

                //memory leak is caused by __cxa_demangle, so wrap return value by shared_ptr
                template<typename T>
                static std::shared_ptr<char> const typeName( T const& t );
            };
        }//Core
    }//SDK

template<typename T>
std::shared_ptr<char> const SSDK::TypeTraits::TypeTraits::typeName( T const& t )
{
#ifdef __GNUC__
    int status;
    return std::shared_ptr<char>(abi::__cxa_demangle( typeid(t).name(), 0, 0, &status ));
#else
    return  std::shared_ptr<char>(typeid(t).name());
#endif
}

#endif // TYPETRAITS_H
