#include "Editor.h"
#include "EditorPropertyUIFactory.h"
#include "EditorPropertyMgr.h"

#include "LogTool.h"

#include "../mainwindow.h"
#include "ui_mainwindow.h"

#include <QtTreePropertyBrowser>
#include <QtVariantProperty>

IMPLEMENT_SINGLETON(Editor::Editor);

namespace Editor
{

Editor::Editor(QObject *parent) : QObject(parent)
{
    PropertyUIFactory::initInstance();
    PropertyMgr::initInstance();
}

Editor::~Editor()
{
    PropertyMgr::finiInstance();
    PropertyUIFactory::finiInstance();
}

void Editor::testProperty()
{
    //load property typedef.
    //Editor::PropertyUIFactory::instance()->registerProertyTemplate(fileName);

    const char * fileName = "property/properties.json";
    if(!PropertyMgr::instance()->loadPropertyFile(fileName))
    {
        LOG_ERROR("Failed to load property file '%s'", fileName);
        return;
    }

    PropertyNode *node = PropertyMgr::instance()->findProperty("Node");
    if(!node)
    {
        LOG_ERROR("Failed to find property 'Node'");
        return;
    }

    MainWindow *window = MainWindow::instance();
    QtVariantEditorFactory *editorFactory = new QtVariantEditorFactory(window);

    QtTreePropertyBrowser *tree = new QtTreePropertyBrowser(window->ui->propertyWidget);
    tree->setFactoryForManager(PropertyUIFactory::instance()->getPropertyMgr(), editorFactory);
    window->ui->propertyWidget->setWidget(tree);

    tree->addProperty(node->getPropertyUI());

}

}
