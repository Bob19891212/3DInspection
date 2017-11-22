#include "measuredobj.hpp"

using namespace Job;

MeasuredObj::MeasuredObj()
{
    this->m_pNextMeasuredObj = nullptr;
    this->m_pPreMeasuredObj = nullptr;
}

MeasuredObj::~MeasuredObj()
{
    this->m_pNextMeasuredObj = nullptr;
    this->m_pPreMeasuredObj = nullptr;
}
