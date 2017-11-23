#include "formatconvertion.hpp"

FormatConvertion::FormatConvertion()
{

}

FormatConvertion::~FormatConvertion()
{

}


string FormatConvertion::intToString(int var)
{
    stringstream string;
    string <<var;
    return string.str();
}

