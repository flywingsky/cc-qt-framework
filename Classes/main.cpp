
#include <QApplication>
#include "mainwindow.h"
#include "AppDelegate.h"

USING_NS_CC;

int main(int argc, char **argv)
{
    // create the application instance
    QApplication a(argc, argv);

    AppDelegate app;

    MainWindow w;
    w.show();

    return a.exec();
}
