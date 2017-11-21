#include <iostream>

#include "sdk/customexception.hpp"
#include "appsetting.hpp"

using namespace std;
using namespace App;


int main()
{
    AppSetting appSetting;
	  appSetting.readAppSetting("app.ini");



    return 0;
}
