//
//  EditorValueTypeMgr.cpp
//  Editor
//
//  Created by C218-pc on 15/7/9.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "EditorPropertyTree.h"
#include "FileTools.h"
#include "uiloader/UIHelper.h"

#include "EditorClassNameMap.h"
#include "EditorPropertyItem.h"
#include "LogTool.h"

#include <qtvariantproperty.h>

DEFINE_LOG_COMPONENT(LOG_PRIORITY_DEBUG, "Property");
IMPLEMENT_SINGLETON(Editor::PropertyTreeMgr);

namespace Editor
{
    
    //////////////////////////////////////////////////////////////////
    ///
    //////////////////////////////////////////////////////////////////
    PropertyTreeNode::PropertyTreeNode()
        : valueRoot_(NULL)
        , parent_(NULL)
        , propertyUI_(NULL)
    {  
    }
    
    PropertyTreeNode::~PropertyTreeNode()
    {
    }
    
    bool PropertyTreeNode::load(rapidjson::Value & config)
    {
        rapidjson::Value *value = &config["name"];
        if(!value->IsString())
        {
            return false;
        }
        name_ = value->GetString();
        
        value = &config["parent"];
        if(value->IsString())
        {
            parent_ = PropertyTreeMgr::instance()->findProperty(value->GetString());
            if(NULL == parent_)
            {
                LOG_ERROR("Failed to find parent '%s' for type '%s'",
                           value->GetString(), name_.c_str());
                return false;
            }
        }
        
        valueRoot_ = new PropertyItemType();
        valueRoot_->type_ = "class";
        valueRoot_->key_ = name_;
        valueRoot_->name_ = name_;
        
        value = &config["property"];
        if(value->IsObject() && !valueRoot_->loadChildren(*value))
        {
            return false;
        }

        propertyUI_ = PropertyItemFactory::instance()->createPropertyByDef(valueRoot_);
        if(NULL == propertyUI_)
        {
            LOG_ERROR("Failed to create ui for property '%s'", name_.c_str());
            return false;
        }

        return true;
    }

    IPropertyItem* PropertyTreeNode::findPropertyItem(const std::string &name)
    {
        if(propertyUI_ != nullptr)
        {
            QString qname(name.c_str());

            QList<QtProperty *> subProperties = propertyUI_->subProperties();
            for(QtProperty *item : subProperties)
            {
                if(qname == item->propertyName())
                {
                    return dynamic_cast<IPropertyItem*>(item);
                }
            }
        }

        return nullptr;
    }
    
    //////////////////////////////////////////////////////////////////
    ///
    //////////////////////////////////////////////////////////////////
    
    PropertyTreeMgr::PropertyTreeMgr()
    {
        loadDefaultClassNameMap(this);
    }
    
    PropertyTreeMgr::~PropertyTreeMgr()
    {
        
    }
    
    bool PropertyTreeMgr::loadPropertyFile(const std::string & filename)
    {
        rapidjson::Document document;
        if(!openJsonFile(filename, document) || !document.IsArray())
        {
            return false;
        }
        
        std::string path = filename;
        size_t split = path.rfind('/');
        if(split != path.npos)
        {
            path.erase(split + 1);
        }
        
        for(rapidjson::Value::ValueIterator it = document.Begin();
            it != document.End(); ++it)
        {
            if(it->IsString())
            {
                if(!loadPropertyFile(path + it->GetString()))
                {
                    return false;
                }
            }
            else if(it->IsObject())
            {
                if(!registerProperty(*it))
                {
                    return false;
                }
            }
        }
        return true;
    }
    
    PropertyTreeNode* PropertyTreeMgr::findProperty(const std::string & name)
    {
        PropertyMap::iterator it = properties_.find(name);
        if(it != properties_.end())
        {
            return it->second;
        }
        return  NULL;
    }
    
    bool PropertyTreeMgr::registerProperty(rapidjson::Value & value)
    {
        PropertyTreeNode *pNode = new PropertyTreeNode();
        if(!pNode->load(value))
        {
            delete pNode;
            return false;
        }
        
        properties_[pNode->getName()] = pNode;
        return true;
    }
    
    const std::string & PropertyTreeMgr::cppNameToUIName(const std::string & className)
    {
        StringMap::iterator it = class2uiName_.find(className);
        if(it != class2uiName_.end())
        {
            return it->second;
        }
        
        return empty_string;
    }
    
    void PropertyTreeMgr::registerUIName(const std::string & className, const std::string & uiName)
    {
        class2uiName_[className] = uiName;
    }
    
} // end namespace Editor
