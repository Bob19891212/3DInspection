#ifndef IDISPOSAL_H
#define IDISPOSAL_H
#include "TypeTraits/functionaltraits.hpp"

    namespace SSDK
    {
        namespace Exception
        {
            class IDisposal
            {
            public:
                IDisposal(){};
                virtual void dispose() = 0;
            };
        }//namespace Exception
    }//namespace SDK

#endif // IDISPOSAL_H
