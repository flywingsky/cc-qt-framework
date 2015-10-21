#include "AppDelegate.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace cocos2d;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *)
{
    cocos2d::CCDirector::getInstance()->end();
    qApp->quit();
}

GLWidget* MainWindow::getGLWidget()
{
    return ui->cocos_widget;
}
