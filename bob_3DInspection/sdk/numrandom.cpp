#include "numrandom.hpp"

using namespace SSDK;

NumRandom::NumRandom()
{

}

NumRandom::~NumRandom()
{

}

int NumRandom::randomInt(int max, int min)
{
    int randomNum = (rand() % (max - min + 1) )+ min;
    return randomNum;
}

double NumRandom::randomDouble(double max, double min)
{
    double randomNum = (rand() / (double)RAND_MAX) * (max - min);
    return randomNum;
}


