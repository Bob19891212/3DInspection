#ifndef MEASUREDOBJ_HPP
#define MEASUREDOBJ_HPP

#include <iostream>

#include "../sdk/customexception.hpp"
#include "../sdk/rectangle.hpp"

using namespace std;

namespace Job
{
    class MeasuredObj
    {
    public:
        MeasuredObj();

    private:
        Job::MeasuredObj *pNextMeasuredObj;
        Job::MeasuredObj *pPreMeasuredObj;

    };
}

#endif // MEASUREDOBJ_HPP
