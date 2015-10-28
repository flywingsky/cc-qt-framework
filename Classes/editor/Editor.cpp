#include "Editor.h"
#include "EditorPropertyItem.h"
#include "EditorPropertyTree.h"
#include "EditorTools.h"
#include "EditorHierarchy.h"
#include "EditorCanvas.h"

#include "uiloader/UILoader.h"
#include "uiloader/UIHelper.h"

#include "LogTool.h"

#include "../mainwindow.h"
#include "ui_mainwindow.h"

#include <QtTreePropertyBrowser>
#include <QtVariantProperty>
#include <QtProperty>

#include <base/CCDirector.h>
#include <2d/CCNode.h>

IMPLEMENT_SINGLETON(Editor::Editor);

namespace Editor
{

namespace
{
    void cloneUIProperty(rapidjson::Value & output, const rapidjson::Value & input, rapidjson::Value::AllocatorType & allocator)
    {
        output.SetObject();

        // clone properties.
        for(rapidjson::Value::ConstMemberIterator it = input.MemberBegin();
            it != input.MemberEnd(); ++it)
        {
            if(strcmp(it->name.GetString(), "children") != 0)
            {
                rapidjson::Value key, val;
                clone_value(key, it->name, allocator);
                clone_value(val, it->value, allocator);
                output.AddMember(key, val, allocator);
            }
        }
    }
}

/*static*/ Editor* Editor::s_instance = nullptr;

Editor::Editor(QObject *parent)
    : QObject(parent)
    , editorFactory_(nullptr)
    , propertyTree_(nullptr)
    , targetConfig_(nullptr)
    , hierarchy_(nullptr)
    , canvas_(nullptr)
{
    PropertyItemFactory::initInstance();
    PropertyTreeMgr::initInstance();
    UILoader::initInstance();

    s_instance = this;
}

Editor::~Editor()
{
    PropertyTreeMgr::finiInstance();
    PropertyItemFactory::finiInstance();
    UILoader::finiInstance();
    s_instance = nullptr;
}

bool Editor::init(cocos2d::Scene *scene)
{
    scene_ = scene;

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
    connect(this, SIGNAL(signalPropertyChange(PropertyParam&)), this, SLOT(onPopertyChange(PropertyParam&)));

    hierarchy_ = new Hierarchy(this, window->ui->treeView);
    connect(hierarchy_, SIGNAL(signalSetTarget(cocos2d::Node*)), this, SLOT(setTargetNode(cocos2d::Node*)));
    connect(this, SIGNAL(signalRootSet(cocos2d::Node*)), hierarchy_, SLOT(onRootSet(cocos2d::Node*)));
    connect(this, SIGNAL(signalTargetSet(cocos2d::Node*)), hierarchy_, SLOT(onTargetSet(cocos2d::Node*)));
    connect(this, SIGNAL(signalNodeCreate(cocos2d::Node*)), hierarchy_, SLOT(onNodeCreate(cocos2d::Node*)));
    connect(this, SIGNAL(signalNodeDelete(cocos2d::Node*)), hierarchy_, SLOT(onNodeDelete(cocos2d::Node*)));
    connect(this, SIGNAL(signalPropertyChange(PropertyParam&)), hierarchy_, SLOT(onPopertyChange(PropertyParam&)));

    canvas_ = new Canvas(this);
    connect(window->ui->cocos_widget, SIGNAL(signalMouseEvent(QMouseEvent*)), canvas_, SLOT(onMouseEvent(QMouseEvent*)));
    connect(window->ui->cocos_widget, SIGNAL(signalKeyEvent(QKeyEvent*)), canvas_, SLOT(onKeyEvent(QKeyEvent*)));
    connect(canvas_, SIGNAL(signalSetTarget(cocos2d::Node*)), this, SLOT(setTargetNode(cocos2d::Node*)));
    connect(this, SIGNAL(signalRootSet(cocos2d::Node*)), canvas_, SLOT(onRootSet(cocos2d::Node*)));
    connect(this, SIGNAL(signalTargetSet(cocos2d::Node*)), canvas_, SLOT(onTargetSet(cocos2d::Node*)));
    connect(this, SIGNAL(signalPropertyChange(PropertyParam&)), canvas_, SLOT(onPopertyChange(PropertyParam&)));


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
    if(rootNode_ == root)
    {
        return;
    }

    if(rootNode_)
    {
        rootNode_->removeFromParent();
    }
    rootNode_ = root;
    if(rootNode_)
    {
        CCAssert(root->getParent() == NULL, "Editor::setRootNode");
        cocos2d::Director::getInstance()->getRunningScene()->addChild(root);
    }

    emit signalRootSet(root);
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
    targetConfig_ = nullptr;

    if(target != nullptr)
    {
        std::string uiName = PropertyTreeMgr::instance()->cppNameToUIName(typeid(*target).name());
        if(uiName.empty())
        {
            LOG_ERROR("doesn't support node type '%s'", typeid(*target).name());
            return;
        }

        // search target configure.
        targetConfig_ = &configures_[target];
        if(targetConfig_->IsNull())
        {
            targetConfig_->SetObject();

            rapidjson::Value jtype(uiName.c_str(), uiName.size(), document_.GetAllocator());
            targetConfig_->AddMember("type", jtype, document_.GetAllocator());
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

    emit signalTargetSet(target);
}

void Editor::onPropertyChange(QtProperty *property, const QVariant &value)
{
    LOG_DEBUG("property change: name = %s, type = %d", property->propertyName().toUtf8().data(), value.type());

    std::string propertyName = property->propertyName().toUtf8().data();

    rapidjson::Value jvalue;
    tvalue2json(jvalue, value, document_.GetAllocator());

    emitTargetPropertyChange(propertyName, jvalue);
}

void Editor::emitTargetPropertyChange(const std::string &name, const rapidjson::Value &value)
{
    PropertyParam param(targetNode_, name, value, *targetConfig_, document_.GetAllocator());
    emit signalPropertyChange(param);
}

void Editor::onPopertyChange(PropertyParam &param)
{
    std::string cppName = typeid(*(param.node)).name();
    const std::string & type = PropertyTreeMgr::instance()->cppNameToUIName(cppName);

    IBaseLoader *loader = UILoader::instance()->findLoader(type);
    if(NULL == loader)
    {
        LOG_ERROR("Failed to find UI loader for type '%s'", type.c_str());
        return;
    }

    const std::string & name = param.name;
    const rapidjson::Value &value = param.value;
    auto & alloc = param.allocator;

    if(loader->setProperty(param.node, name, value, param.properties))
    {
        rapidjson::Value temp;
        clone_value(temp, value, alloc);
        rapidjson::Value & slot = param.properties[name.c_str()];
        if(slot.IsNull())
        {
            targetConfig_->AddMember(name.c_str(), alloc, temp, alloc);
        }
        else
        {
            slot = temp;
        }
    }
}

void Editor::createNode(cocos2d::Node *node)
{
    CCAssert(node, "Editor::createNode");
    const std::string & type = PropertyTreeMgr::instance()->cppNameToUIName(typeid(*node).name());
    if(type.empty())
    {
        LOG_ERROR("Doesn't support node('%s').", typeid(*node).name());
        return;
    }

    if(!rootNode_)
    {
        setRootNode(node);
    }
    else if(!targetNode_)
    {
        rootNode_->addChild(node);
    }
    else
    {
        targetNode_->addChild(node);
    }

    rapidjson::Value config;
    config.SetObject();

    rapidjson::Value jtype(type.c_str(), type.size(), document_.GetAllocator());
    config.AddMember("type", jtype, document_.GetAllocator());

    configures_[node] = config;


    emit signalNodeCreate(node);

    setTargetNode(node);
}

bool Editor::loadLayout(const std::string & fileName)
{
    clearLayout();

    if(!openJsonFile(fileName, document_) || !document_.IsObject())
    {
        LOG_ERROR("Failed to open layout file '%s'", fileName.c_str());
        document_.SetNull();
        return false;
    }

    if(!UILoader::instance()->upgradeLayoutFile(document_))
    {
        LOG_ERROR("Failed to upgrade layout file '%s'", fileName.c_str());
        document_.SetNull();
        return false;
    }

    cocos2d::Node * pNode = UILoader::instance()->loadLayoutFromStream(document_);
    if(pNode != NULL)
    {
        loadNodeConfigure(pNode, document_);
    }

    setRootNode(pNode);
    setTargetNode(pNode);
    return pNode != NULL;
}

bool Editor::saveLayout(const std::string & fileName)
{
    if(!rootNode_)
    {
        return false;
    }
    saveNodeConfigure(rootNode_, document_);

    return UILoader::instance()->saveLayoutToFile(fileName, document_);
}

void Editor::clearLayout()
{
    setTargetNode(nullptr);
    setRootNode(nullptr);

    configures_.clear();
    document_.SetNull();
}

bool Editor::loadNodeConfigure(cocos2d::Node *node, const rapidjson::Value & value)
{
    CCAssert(value.IsObject(), "Editor::loadNodeConfigure");

    // clone properties.
    rapidjson::Value & config = configures_[node];
    cloneUIProperty(config, value, document_.GetAllocator());

    // clone children configure.
    const rapidjson::Value & jchildren = value["children"];
    if(jchildren.IsArray())
    {
        const auto & children = node->getChildren();
        unsigned nChildren = std::min((unsigned)children.size(), jchildren.Size());
        for(unsigned i = 0; i < nChildren; ++i)
        {
            const rapidjson::Value & jchild = jchildren[i];
            if(jchild.IsObject())
            {
                cocos2d::Node* child = children.at(i);
                loadNodeConfigure(child, jchild);
            }
        }
    }

    return true;
}

bool Editor::saveNodeConfigure(cocos2d::Node *node, rapidjson::Value & out)
{
    CCAssert(node != NULL, "Editor::saveNodeConfigure");

    auto it = configures_.find(node);
    if(it == configures_.end())
    {
        return false;
    }

    // clone property
    const rapidjson::Value &config = it->second;
    cloneUIProperty(out, config, document_.GetAllocator());

    // clone children.
    const auto & children = node->getChildren();
    if(!children.empty())
    {
        rapidjson::Value jchildren;
        jchildren.SetArray();
        jchildren.Reserve(children.size(), document_.GetAllocator());

        for(unsigned i = 0; i < children.size(); ++i)
        {
            rapidjson::Value jchild;
            cocos2d::Node* child = children.at(i);

            if(saveNodeConfigure(child, jchild))
            {
                jchildren.PushBack(jchild, document_.GetAllocator());
            }
        }

        out.AddMember("children", jchildren, document_.GetAllocator());
    }

    return true;
}

}
