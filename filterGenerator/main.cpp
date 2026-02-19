#include "mainwindow.h"
#include <QtWidgets/QApplication>

#define APP_TITILE "Filter Generator V2.0.0 By wysaid"

bool enableGLFunction(void* arg)
{
    if (arg)
    {
        ((MainWindow*)arg)->useCanvasContext();
        return true;
    }
    return false;
}

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.setMinimumSize(640, 480);
    w.setWindowTitle(APP_TITILE);
    CGE::cgeSetGLContextEnableFunction(enableGLFunction, &w);
    return a.exec();
}
