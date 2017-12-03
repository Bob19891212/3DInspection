#include "formatconvertion.hpp"

using namespace SSDK;

FormatConvertion::FormatConvertion()
{

}

FormatConvertion::~FormatConvertion()
{

}


string FormatConvertion::intToString(int value)
{
    try
    {
        //定义一个字符串流对象
        stringstream string;
        //将整形数值赋值给字符串流
        string <<value;
        //将转换后的字符串作为返回值
        return string.str();
    }
    catch(const exception &ex)
    {
        THROW_EXCEPTION(ex.what());
    }
}
