//
//  EditorPropertyValue.cpp
//  Editor
//
//  Created by C218-pc on 15/7/20.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "EditorPropertyUIFactory.h"
#include "EditorTools.h"
#include "EditorPropertyUI.h"
#include "LogTool.h"

#include <uilib/UIHelper.h>

DEFINE_LOG_COMPONENT(LOG_PRIORITY_DEBUG, "PropertyUIFactory");
IMPLEMENT_SINGLETON(Editor::PropertyUIFactory);

namespace Editor
{
    PropertyTypedef::PropertyTypedef()
    {
    }
    
    PropertyTypedef::~PropertyTypedef()
    {
        for(std::vector<PropertyTypedef*>::iterator it = m_items.begin();
            it != m_items.end(); ++it)
        {
            delete (*it);
        }
    }
    
    bool PropertyTypedef::loadChildren(const rapidjson::Value &config)
    {
        for(rapidjson::Value::ConstMemberIterator it = config.MemberBegin();
            it != config.MemberEnd(); ++it)
        {
            PropertyTypedef *child = new PropertyTypedef();
            child->m_key = it->name.GetString();
            
            if(!child->loadValue(it->value))
            {
                delete child;
                return false;
            }
            this->m_items.push_back(child);
        }
        
        return true;
    }
    
    bool PropertyTypedef::loadTypedef(const rapidjson::Value & config)
    {
        const rapidjson::Value *value = &config["type"];
        if(!value->IsString())
        {
            return false;
        }
        m_type = value->GetString();
        
        value = &config["skin"];
        if(value->IsString())
        {
            m_skin = value->GetString();
        }
        
        value = &config["item"];
        if(value->IsArray())
        {
            for (rapidjson::Value::ConstValueIterator it = value->Begin(); it != value->End(); ++it)
            {                
                PropertyTypedef *item = new PropertyTypedef();
                if(!item->loadValue(*it))
                {
                    delete item;
                    return false;
                }
                
                m_items.push_back(item);
            }
        }
        
        value = &config["args"];
        if(!value->IsNull())
        {
            json2tvalue(m_extraArgs, *value);
        }
        return true;
    }
    
    bool PropertyTypedef::loadValue(const rapidjson::Value & config)
    {
        if(!config.IsArray() || config.Size() < 3)
        {
            LOG_ERROR("Invalid value type");
            return false;
        }
        
        const rapidjson::Value * value = &config[0u];
        if(!value->IsString())
        {
            LOG_ERROR("Invalid item type, #0 must be a string type.");
            return false;
        }
        m_type = value->GetString();
        
        // store default value.
        json2tvalue(m_defaultValue, config[1]);
        
        value = &config[2];
        if(!value->IsString())
        {
            LOG_ERROR("Invalid item type, #2 must be a string type.");
            return false;
        }
        m_name = value->GetString();
        
        if(config.Size() > 3)
        {
            value = &config[3];
            if(value->IsString())
            {
                m_desc = value->GetString();
            }
        }
        
        return true;
    }
    
    //////////////////////////////////////////////////////////////////
    ///
    //////////////////////////////////////////////////////////////////
    PropertyUIFactory::PropertyUIFactory()
    {
#define REG_BASIC_PROPERTY(NAME, CLASS)\
registerBasicProperty(NAME, (SEL_CreatePropertyUI)CLASS::create)
        
        REG_BASIC_PROPERTY("bool", BoolProperty);
        REG_BASIC_PROPERTY("int", IntProperty);
        REG_BASIC_PROPERTY("float", FloatProperty);
        REG_BASIC_PROPERTY("string", StringProperty);
        REG_BASIC_PROPERTY("file", FileProperty);
        REG_BASIC_PROPERTY("image", ImageProperty);
        REG_BASIC_PROPERTY("select", SelectProperty);
        REG_BASIC_PROPERTY("list", ListProperty);
        REG_BASIC_PROPERTY("dict", DictProperty);
        REG_BASIC_PROPERTY("class", ClassProperty);
        
#undef REG_BASIC_PROPERTY
    }
    
    PropertyUIFactory::~PropertyUIFactory()
    {
        
    }
    
    IPropertyUI * PropertyUIFactory::createProperty(const std::string & name)
    {
        IPropertyUI *p = createBasicProperty(name);
        if(NULL == p)
        {
            p = createCombinedProperty(name);
        }
        return p;
    }
    
    void PropertyUIFactory::registerBasicProperty(const std::string & name, SEL_CreatePropertyUI method)
    {
        if(!m_factory.insert(std::pair<std::string, SEL_CreatePropertyUI>(name, method)).second)
        {
            LOG_ERROR("The property %s has been exist.", name.c_str());
        }
    }
    
    bool PropertyUIFactory::registerProertyTemplate(const std::string & filename)
    {
        rapidjson::Document document;
        if(!openJsonFile(filename, document))
        {
            return false;
        }
        
        typedef std::pair<std::string, PropertyTypedef*> DeclarePair;
        
        for (rapidjson::Value::MemberIterator it = document.MemberBegin(); 
             it != document.MemberEnd(); ++it)
        {
            PropertyTypedef * declare = new PropertyTypedef();
            if(!declare->loadTypedef(it->value))
            {
                delete declare;
                return false;
            }
            
            std::pair<PropertyTypedefMap::iterator, bool> ret = m_declares.insert(DeclarePair(it->name.GetString(), declare));
            if(!ret.second)
            {
                LOG_ERROR("The old declare '%s' will be covered.", it->name.GetString());
                
                delete ret.first->second;
                ret.first->second = declare;
            }
        }
        
        return true;
    }
    
    IPropertyUI* PropertyUIFactory::createBasicProperty(const std::string & name)
    {
        PropertyFactory::iterator it = m_factory.find(name);
        if(it != m_factory.end())
        {
            return it->second();
        }
        
        return NULL;
    }
    
    IPropertyUI* PropertyUIFactory::createCombinedProperty(const std::string & name)
    {
        PropertyTypedefMap::iterator it = m_declares.find(name);
        if(it == m_declares.end()) return NULL;
        
        return createPropertyByType(it->second);
    }
    
    IPropertyUI* PropertyUIFactory::createPropertyByType(PropertyTypedef *declare)
    {
        IPropertyUI * root = createProperty(declare->m_type);
        if(NULL == root)
        {
            LOG_ERROR("Failed to create property ui for type '%s'", declare->m_type.c_str());
            return NULL;
        }
        
        root->setKey(declare->m_key);
        root->setText(declare->m_name);
        root->setDescription(declare->m_desc);
        
        if(!declare->m_extraArgs.is_null())
        {
            root->setExtraArgs(declare->m_extraArgs);
        }
        
        root->setDefault(declare->m_defaultValue);
        
        if(!declare->m_skin.empty())
        {
            root->setSkin(declare->m_skin);
        }
        
        if(!declare->m_items.empty())
        {
            GroupProperty *group = dynamic_cast<GroupProperty*>(root);
            if(NULL == group)
            {
                LOG_ERROR("The property '%s' must be group type.", declare->m_key.c_str());
                return NULL;
            }
            
            for(std::vector<PropertyTypedef*>::iterator it = declare->m_items.begin();
                it != declare->m_items.end(); ++it)
            {
                IPropertyUI *item = createPropertyByType(*it);
                if(NULL == item)
                {
                    LOG_ERROR("Failed to create property value for '%s'", (*it)->m_key.c_str());
                    return NULL;
                }
                
                group->addProperty(item);
            }
        }
        
        root->onBind();
        return root;
    }
    
}
