#include "Editor.h"
#include "EditorPropertyItemFactory.h"
#include "EditorPropertyMgr.h"
#include "EditorTools.h"
#include "uiloader/UILoader.h"

#include "LogTool.h"

#include "../mainwindow.h"
#include "ui_mainwindow.h"

#include <QtTreePropertyBrowser>
#include <QtVariantProperty>
#include <QtProperty>

#include <2d/CCNode.h>

IMPLEMENT_SINGLETON(Editor::Editor);

namespace Editor
{

Editor::Editor(QObject *parent)
    : QObject(parent)
    , editorFactory_(nullptr)
    , propertyTree_(nullptr)
{
    PropertyItemFactory::initInstance();
    PropertyTreeMgr::initInstance();
    UILoader::initInstance();
}

Editor::~Editor()
{
    PropertyTreeMgr::finiInstance();
    PropertyItemFactory::finiInstance();
    UILoader::finiInstance();
}

bool Editor::init()
{
    //load property typedef.
    const char *valueFile = "property/values.json";
    if(!PropertyItemFactory::instance()->registerProertyTemplate(valueFile))
    {
        LOG_ERROR("Failed to load property value typedef file '%s'", valueFile);
        return false;
    }

    const char * fileName = "property/properties.json";
    if(!PropertyTreeMgr::instance()->loadPropertyFile(fileName))
    {
        LOG_ERROR("Failed to load property file '%s'", fileName);
        return false;
    }

    MainWindow *window = MainWindow::instance();

    editorFactory_ = new QtVariantEditorFactory(window);
    propertyTree_ = new QtTreePropertyBrowser(window->ui->propertyWidget);

    QtVariantPropertyManager *propertyMgr = PropertyItemFactory::instance()->getPropertyMgr();
    propertyTree_->setFactoryForManager(propertyMgr, editorFactory_);
    window->ui->propertyWidget->setWidget(propertyTree_);

    connect(propertyMgr, SIGNAL(valueChanged(QtProperty*,QVariant)), this, SLOT(onPropertyChange(QtProperty*,QVariant)));

    //testProperty();
    return true;
}

void Editor::testProperty()
{
    PropertyTreeNode *node = PropertyTreeMgr::instance()->findProperty("Node");
    if(!node)
    {
        LOG_ERROR("Failed to find property 'Node'");
        return;
    }

    propertyTree_->addProperty(node->getPropertyItem());
}

void Editor::setRootNode(cocos2d::Node *root)
{
    rootNode_ = root;
}

void Editor::setTargetNode(cocos2d::Node *target)
{
    if(targetNode_ == target)
    {
        return;
    }

    // cleanup old property.
    if(targetNode_)
    {
        propertyTree_->clear();
    }

    targetNode_ = target;

    if(target != nullptr)
    {
        std::string uiName = PropertyTreeMgr::instance()->cppNameToUIName(typeid(*target).name());
        if(uiName.empty())
        {
            LOG_ERROR("doesn't support node type '%s'", typeid(*target).name());
            return;
        }

        PropertyTreeNode *node = PropertyTreeMgr::instance()->findProperty(uiName);
        if(nullptr == node)
        {
            LOG_ERROR("Failed to find property description for ui type '%s'", uiName.c_str());
            return;
        }

        std::vector<PropertyTreeNode*> stack;
        do
        {
            stack.push_back(node);
            node = node->getParent();
        }while(node != nullptr);

        for(auto it = stack.rbegin(); it != stack.rend(); ++it)
        {
            propertyTree_->addProperty((*it)->getPropertyItem());
        }
    }
}

void Editor::onPropertyChange(QtProperty *property, const QVariant &value)
{
    LOG_DEBUG("property change: name = %s, type = %d", property->propertyName().toUtf8().data(), value.type());

    if(targetNode_)
    {

        const std::string & type = PropertyTreeMgr::instance()->cppNameToUIName(typeid(*targetNode_).name());
        CCAssert(!type.empty(), "Editor::onPropertyChange");

        IBaseLoader *loader = UILoader::instance()->findLoader(type);
        if(NULL == loader)
        {
            LOG_ERROR("Failed to find UI loader for type '%s'", type.c_str());
            return;
        }

        rapidjson::Value jvalue;
        tvalue2json(jvalue, value, document_.GetAllocator());

        std::string propertyName = property->propertyName().toUtf8().data();

        rapidjson::Value & properties = document_; //(*m_selectedConfig)["property"];
        loader->setProperty(targetNode_, propertyName, jvalue, properties);
    }
}

}
