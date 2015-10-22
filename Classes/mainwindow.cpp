#include "AppDelegate.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <qttreepropertybrowser.h>
#include <qtvariantproperty.h>

using namespace cocos2d;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QtVariantPropertyManager *propertyMgr = new QtVariantPropertyManager(this);
    QtVariantEditorFactory *editorFactory = new QtVariantEditorFactory(this);

    QtTreePropertyBrowser *tree = new QtTreePropertyBrowser(ui->propertyWidget);
    tree->setFactoryForManager(propertyMgr, editorFactory);
    ui->propertyWidget->setWidget(tree);

    QtVariantProperty *item;

    item = propertyMgr->addProperty(QVariant::String, tr("type"));
    item->setValue(tr("Node"));
    item->setEnabled(false);
    tree->addProperty(item);

    item = propertyMgr->addProperty(QVariant::String, tr("name"));
    item->setValue(tr("name_0"));
    tree->addProperty(item);

    item = propertyMgr->addProperty(QVariant::Double, tr("position x"));
    item->setValue(QVariant(99.0));
    tree->addProperty(item);

    item = propertyMgr->addProperty(QVariant::Double, tr("position y"));
    item->setValue(QVariant(88.0));
    tree->addProperty(item);
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
