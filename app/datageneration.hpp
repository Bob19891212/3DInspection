#ifndef DATAGENERATION_HPP
#define DATAGENERATION_HPP

#include <time.h>

#include "../sdk/customexception.hpp"
#include "../job/inspectiondata.hpp"
#include "../sdk/formatconvertion.hpp"

using namespace std;
using namespace Job;
using namespace SSDK;

namespace App
{
    class DataGeneration
    {
    public:
        DataGeneration();

        ~DataGeneration();

        void generateData(int size,
                          MeasuredObj measuredObjArr[]);
    };
}

#endif // DATAGENERATION_HPP
