#include <iostream>

#include "app/config.hpp"
#include "app/mainwindow.hpp"

using namespace std;
using namespace App;
using namespace Job;

#define JOB_DIR "./data/"

int main()
{
    //定义一个类对象
    Config config;
    config.readConfigFiles();

    //读取程式
    MainWindow mainWindow;
    mainWindow.loadJob(JOB_DIR);

    return 0;
}
