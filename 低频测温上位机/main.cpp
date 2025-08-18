#include "mainwindow.h"
#include "splashwindow.h"

#include <QApplication>
#include "crc.h"

/*
注释 + 调试，需要发射&接收板
*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /* 刷新界面 start */
    SplashWindow splash;
    splash.show();
    while(splash.progressBar->value()!=100)
    {
        int i =splash.progressBar->value()+rand()%20+10;
        if(i>100)
            splash.progressBar->setValue(100);
        else
            splash.progressBar->setValue(i);
        a.processEvents();
        Sleep(500);
    }
    splash.close();
    /* 刷新界面 end */

    /* 主窗口 */
    MainWindow w;
    w.show();

    return a.exec();
}
