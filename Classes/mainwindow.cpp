#include "AppDelegate.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <qttreepropertybrowser.h>
#include <qtvariantproperty.h>

using namespace cocos2d;

/*static*/ MainWindow* MainWindow::s_instance = nullptr;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    s_instance = this;

    ui->setupUi(this);


#if 0
    QtVariantPropertyManager *propertyMgr = new QtVariantPropertyManager(this);
    QtVariantEditorFactory *editorFactory = new QtVariantEditorFactory(this);

    QtTreePropertyBrowser *tree = new QtTreePropertyBrowser(ui->propertyWidget);
    tree->setFactoryForManager(propertyMgr, editorFactory);
    ui->propertyWidget->setWidget(tree);

    QtVariantProperty *group, *item;

    // type
    item = propertyMgr->addProperty(QVariant::String, tr("type"));
    item->setValue(tr("Node"));
    item->setAttribute(QLatin1String("readOnly"), true);
    tree->addProperty(item);

    // group 1
    group = propertyMgr->addProperty(QtVariantPropertyManager::groupTypeId(), tr("Node"));

    item = propertyMgr->addProperty(QVariant::String, tr("name"));
    item->setValue(tr("name_0"));
    group->addSubProperty(item);

    item = propertyMgr->addProperty(QVariant::Double, tr("position x"));
    item->setValue(QVariant(99.0));
    group->addSubProperty(item);

    item = propertyMgr->addProperty(QVariant::Double, tr("position y"));
    item->setValue(QVariant(88.0));
    group->addSubProperty(item);

    tree->addProperty(group);

    // group 2
    group = propertyMgr->addProperty(propertyMgr->groupTypeId(), tr("Sprite"));

    item = propertyMgr->addProperty(QVariant::String, tr("image"));
    item->setValue(tr("HelloWorld.png"));
    group->addSubProperty(item);

    tree->addProperty(group);
#endif
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
