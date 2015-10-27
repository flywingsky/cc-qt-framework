#include "AppDelegate.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "editor/Editor.h"

/*static*/ MainWindow* MainWindow::s_instance = nullptr;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    s_instance = this;

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;

    if(s_instance == this)
    {
        s_instance = nullptr;
    }
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

void MainWindow::on_actionNode_triggered()
{
    Editor::Editor::instance()->createNode(cocos2d::Node::create());
}

void MainWindow::on_actionSprite2D_triggered()
{
    Editor::Editor::instance()->createNode(cocos2d::Sprite::create("HelloWorld.png"));
}
