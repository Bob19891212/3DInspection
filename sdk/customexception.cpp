#include "customexception.hpp"

using namespace SSDK;

CustomException::CustomException(std::string &message)
{
    this->m_originalMsg = message;
}
