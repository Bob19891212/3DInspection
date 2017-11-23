#include "customexception.hpp"

using namespace SSDK;

CustomException::CustomException()
{
    this->m_originalMsg = {""};
}

CustomException::CustomException(std::string &message)
{
    this->m_originalMsg = message;
}

CustomException::~CustomException()
{

}
