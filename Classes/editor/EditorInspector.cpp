#include "EditorInspector.h"
#include "Editor.h"
#include "EditorPropertyItem.h"
#include "EditorPropertyTree.h"
#include "EditorTools.h"
#include "EditorPropertyDefault.h"

#include "uiloader/UILoader.h"
#include "LogTool.h"

#include <QtTreePropertyBrowser>
#include <QtVariantProperty>
#include <QtProperty>

#include <QDockWidget>

namespace Editor
{

    Inspector::Inspector(QObject *parent, QDockWidget *view)
        : QObject(parent)
        , propertyType_(nullptr)
    {
        editorFactory_ = new QtVariantEditorFactory(this);
        propertyTree_ = new QtTreePropertyBrowser(view);

        QtVariantPropertyManager *propertyMgr = PropertyItemFactory::instance()->getPropertyMgr();
        propertyTree_->setFactoryForManager(propertyMgr, editorFactory_);
        view->setWidget(propertyTree_);

        propertyType_ = propertyMgr->addProperty(QVariant::String, "type");
        propertyType_->setAttribute("readOnly", true);

        connect(propertyMgr, SIGNAL(valueChanged(QtProperty*,QVariant)), this, SLOT(onPropertyChange(QtProperty*,QVariant)));
    }


    void Inspector::onTargetSet(cocos2d::Node *target)
    {
        if(targetNode_ == target)
        {
            return;
        }

        // cleanup old property.
        if(targetNode_)
        {
            propertyTree_->clear();
            propertyGroup_.clear();
            name2property_.clear();
        }

        targetNode_ = target;
        targetConfig_ = Editor::instance()->getTargetConfig();

        if(!targetNode_)
        {
            return;
        }

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

        do
        {
            propertyGroup_.push_back(node);
            node = node->getParent();
        }while(node != nullptr);

        propertyTree_->addProperty(propertyType_);
        for(auto it = propertyGroup_.rbegin(); it != propertyGroup_.rend(); ++it)
        {
            propertyTree_->addProperty((*it)->getPropertyItem());
        }

        bindNameAndProperty();
        applyDataToView();
    }

    void Inspector::setTargetProperty(const std::string &name, const rapidjson::Value &value)
    {
        auto it = name2property_.find(name);
        if(it == name2property_.end())
        {
            return;
        }

        QtVariantProperty *property = dynamic_cast<QtVariantProperty*>(it->second);
        if(property != nullptr)
        {
            int valueType = property->valueType();
            QVariant qvalue;
            json2tvalue(qvalue, value, valueType);

            QtVariantPropertyManager *propertyMgr = PropertyItemFactory::instance()->getPropertyMgr();
            propertyMgr->setValue(property, qvalue);
        }
    }

    void Inspector::onPropertyChange(QtProperty *property, const QVariant &value)
    {
        std::string name = property->propertyName().toUtf8().data();

        rapidjson::Value::AllocatorType & alloc = Editor::instance()->getAllocator();

        rapidjson::Value jvalue;
        tvalue2json(jvalue, value, alloc);

        PropertyParam param(targetNode_, name, jvalue, *targetConfig_, alloc);
        emit signalPropertyChange(param);
    }

    void Inspector::onPopertyChange(PropertyParam &param)
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
        auto & alloc = param.allocator();

        if(loader->setProperty(param))
        {
            rapidjson::Value temp;
            clone_value(temp, param.value, alloc);
            rapidjson::Value & slot = param.properties[name.c_str()];
            if(slot.IsNull())
            {
                param.properties.AddMember(name.c_str(), alloc, temp, alloc);
            }
            else
            {
                slot = temp;
            }
        }
    }

    void Inspector::bindNameAndProperty()
    {
        name2property_.clear();
        name2property_["type"] = propertyType_;

        for(PropertyTreeNode *node : propertyGroup_)
        {
            QList<QtProperty *> subProperties = node->getPropertyItem()->subProperties();
            for(QtProperty *item : subProperties)
            {
                std::string name = item->propertyName().toUtf8().data();
                name2property_[name] = item;
            }
        }
    }

    void Inspector::applyDataToView()
    {
        QtVariantPropertyManager *propertyMgr = PropertyItemFactory::instance()->getPropertyMgr();
        disconnect(propertyMgr, SIGNAL(valueChanged(QtProperty*,QVariant)), this, SLOT(onPropertyChange(QtProperty*,QVariant)));

        for(PropertyTreeNode *node : propertyGroup_)
        {
            const rapidjson::Value *config = PropertyDefault::instance()->name2config(node->getName());
            if(config != nullptr)
            {
                applyConfigToView(*config);
            }
        }

        applyConfigToView(*targetConfig_);

        connect(propertyMgr, SIGNAL(valueChanged(QtProperty*,QVariant)), this, SLOT(onPropertyChange(QtProperty*,QVariant)));
    }

    void Inspector::applyConfigToView(const rapidjson::Value &config)
    {
        for(rapidjson::Value::ConstMemberIterator it = config.MemberBegin();
            it != config.MemberEnd(); ++it)
        {
            setTargetProperty(it->name.GetString(), it->value);
        }
    }

} // end namespace Editor
