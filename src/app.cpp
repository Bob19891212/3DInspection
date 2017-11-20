#include <iostream>


#include "sdk/customexception.hpp"

using namespace std;


void func()
{
    THROW_EXCEPTION("Argument invaild!");
}

int main()
{
    func();
    return 0;
}
