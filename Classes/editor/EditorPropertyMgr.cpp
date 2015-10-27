//
//  EditorValueTypeMgr.cpp
//  Editor
//
//  Created by C218-pc on 15/7/9.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "EditorPropertyMgr.h"
#include "FileTools.h"
#include "uiloader/UIHelper.h"

#include "EditorClassNameMap.h"
#include "EditorPropertyItemFactory.h"
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
        : m_valueRoot(NULL)
        , m_parent(NULL)
        , m_propertyUI(NULL)
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
        m_name = value->GetString();
        
        value = &config["parent"];
        if(value->IsString())
        {
            m_parent = PropertyTreeMgr::instance()->findProperty(value->GetString());
            if(NULL == m_parent)
            {
                LOG_ERROR("Failed to find parent '%s' for type '%s'",
                           value->GetString(), m_name.c_str());
                return false;
            }
        }
        
        m_valueRoot = new PropertyItemType();
        m_valueRoot->m_type = "class";
        m_valueRoot->m_key = m_name;
        m_valueRoot->m_name = m_name;
        
        value = &config["property"];
        if(value->IsObject() && !m_valueRoot->loadChildren(*value))
        {
            return false;
        }

        m_propertyUI = PropertyItemFactory::instance()->createPropertyByDef(m_valueRoot);
        if(NULL == m_propertyUI)
        {
            LOG_ERROR("Failed to create ui for property '%s'", m_name.c_str());
            return false;
        }

        return true;
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
        PropertyMap::iterator it = m_properties.find(name);
        if(it != m_properties.end())
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
        
        m_properties[pNode->getName()] = pNode;
        return true;
    }
    
    const std::string & PropertyTreeMgr::cppNameToUIName(const std::string & className)
    {
        StringMap::iterator it = m_class2uiName.find(className);
        if(it != m_class2uiName.end())
        {
            return it->second;
        }
        
        return empty_string;
    }
    
    void PropertyTreeMgr::registerUIName(const std::string & className, const std::string & uiName)
    {
        m_class2uiName[className] = uiName;
    }
    
} // end namespace Editor
