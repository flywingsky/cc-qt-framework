#include "AppDelegate.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "editor/Editor.h"
#include "editor/EditorSprite3D.h"

#include <QFileDialog>

/*static*/ MainWindow* MainWindow::s_instance = nullptr;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    s_instance = this;

    ui->setupUi(this);

    new Editor::Editor(this);
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

void MainWindow::on_actionCreateSprite2D_triggered()
{
    ui->cocos_widget->makeCurrent();
    Editor::Editor::instance()->createNode(cocos2d::Sprite::create("HelloWorld.png"));
}

void MainWindow::on_actionCreateNode_triggered()
{
    ui->cocos_widget->makeCurrent();
    Editor::Editor::instance()->createNode(cocos2d::Node::create());
}

void MainWindow::on_actionFileOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select File"), "", "Editor Files (*.json)");
    if(!fileName.isEmpty())
    {
        ui->cocos_widget->makeCurrent();
        Editor::Editor::instance()->loadLayout(fileName.toUtf8().data());
    }
}

void MainWindow::on_actionFileSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save"), "", "Editor Files (*.json)");
    if(!fileName.isEmpty())
    {
        ui->cocos_widget->makeCurrent();
        Editor::Editor::instance()->saveLayout(fileName.toUtf8().data());
    }
}

void MainWindow::on_actionFileClear_triggered()
{
    ui->cocos_widget->makeCurrent();
    Editor::Editor::instance()->clearLayout();
}

void MainWindow::on_actionCreateSprite3D_triggered()
{
    ui->cocos_widget->makeCurrent();
    Editor::Editor::instance()->createNode(EditorSprite3D::create("res/tortoise.c3b"));
}
