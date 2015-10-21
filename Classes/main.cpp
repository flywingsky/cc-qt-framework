
#include <QApplication>
#include <QOpenGLWidget>
#include "mainwindow.h"


#include "AppDelegate.h"

USING_NS_CC;

int main(int argc, char **argv)
{
    // create the application instance
    QApplication a(argc, argv);

    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    //format.setVersion(3, 2);
    //format.setRenderableType(QSurfaceFormat::OpenGLES);
    QSurfaceFormat::setDefaultFormat(format);

    AppDelegate app;

    MainWindow w;
    w.show();

    return a.exec();
}

