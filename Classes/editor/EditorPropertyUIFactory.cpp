//
//  EditorPropertyValue.cpp
//  Editor
//
//  Created by C218-pc on 15/7/20.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "EditorPropertyUIFactory.h"
#include "EditorTools.h"
#include "LogTool.h"

#include "uiloader/UIHelper.h"

#include <qtvariantproperty.h>

DEFINE_LOG_COMPONENT(LOG_PRIORITY_DEBUG, "PropertyUIFactory");
IMPLEMENT_SINGLETON(Editor::PropertyUIFactory);

namespace Editor
{
    class PropertyCreator
    {
    public:
        virtual ~PropertyCreator(){}
        virtual IPropertyUI* create(PropertyUIFactory *factory) = 0;
    };

    namespace
    {
        bool parseAttributeValue(QVariant & qvalue, const rapidjson::Value & jvalue)
        {
            if(jvalue.IsArray() && jvalue.Size() == 2)
            {
                std::string type = jvalue[0u].GetString();
                int itype = PropertyUIFactory::instance()->name2type(type);

                json2tvalue(qvalue, jvalue[1], itype);
                return true;
            }
            return false;
        }

        template<int T>
        IPropertyUI* createQtPropertyUI(QtVariantPropertyManager *mgr)
        {
            return mgr->addProperty(T);
        }

        IPropertyUI* createGroupPropertyUI(QtVariantPropertyManager *mgr)
        {
            return mgr->addProperty(mgr->groupTypeId());
        }

        class BuiltinPropertyCreator : public PropertyCreator
        {
            SEL_CreatePropertyUI m_method;
        public:
            BuiltinPropertyCreator(SEL_CreatePropertyUI method)
                : m_method(method)
            {}

            ~BuiltinPropertyCreator()
            {}

            virtual IPropertyUI* create(PropertyUIFactory *factory) override
            {
                return m_method(factory->getPropertyMgr());
            }
        };

        class CombinedPropertyCreator : public PropertyCreator
        {
            PropertyTypedef*    m_declare;
        public:
            CombinedPropertyCreator(PropertyTypedef *declare)
                : m_declare(declare)
            {}

            ~CombinedPropertyCreator()
            {
                delete m_declare;
            }

            virtual IPropertyUI* create(PropertyUIFactory *factory) override
            {
                return factory->createPropertyByDef(m_declare);
            }
        };
    }

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
        
        value = &config["attribute"];
        if(value->IsObject())
        {
            for(rapidjson::Value::ConstMemberIterator it = value->MemberBegin();
                it != value->MemberEnd(); ++it)
            {
                QString qkey(it->name.GetString());
                QVariant qvalue;
                parseAttributeValue(qvalue, it->value);
                m_attributes.insert(qkey, qvalue);
            }
        }
        return true;
    }
    
    bool PropertyTypedef::loadValue(const rapidjson::Value & config)
    {
        if(!config.IsArray() || config.Size() < 2)
        {
            LOG_ERROR("Invalid value type");
            return false;
        }
        
        const rapidjson::Value * value = nullptr;

        value = &config[0u];
        if(!value->IsString())
        {
            LOG_ERROR("Invalid item type, #0 must be a string type.");
            return false;
        }
        m_type = value->GetString();

        value = &config[1];
        if(!value->IsString())
        {
            LOG_ERROR("Invalid item type, #1 must be a string type.");
            return false;
        }
        m_name = value->GetString();
        
        if(config.Size() > 2)
        {
            value = &config[2];
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
        : m_propertyMgr(new QtVariantPropertyManager())
    {
#define REG_PROPERTY(NAME, TYPE) \
    registerBasicProperty(NAME, TYPE, createQtPropertyUI<TYPE>);

        REG_PROPERTY("bool", QVariant::Bool);
        REG_PROPERTY("int", QVariant::Int);
        REG_PROPERTY("float", QVariant::Double);
        REG_PROPERTY("string", QVariant::String);
        REG_PROPERTY("Vec2", QVariant::Vector2D);
        REG_PROPERTY("Vec3", QVariant::Vector3D);
        REG_PROPERTY("Vec4", QVariant::Vector4D);
        REG_PROPERTY("color", QVariant::Color);
        REG_PROPERTY("size", QVariant::Size);
        REG_PROPERTY("sizeF", QVariant::SizeF);
        REG_PROPERTY("rect", QVariant::Rect);
        REG_PROPERTY("rectF", QVariant::RectF);
        REG_PROPERTY("point", QVariant::Point);
        REG_PROPERTY("pointF", QVariant::PointF);
        REG_PROPERTY("file", QVariant::String);
        REG_PROPERTY("image", QVariant::String);
        REG_PROPERTY("select", QVariant::List);
        REG_PROPERTY("list", QVariant::List);
        REG_PROPERTY("dict", QVariant::Map);
#undef REG_PROPERTY

        registerBasicProperty("class", QtVariantPropertyManager::groupTypeId(), createGroupPropertyUI);
    }
    
    PropertyUIFactory::~PropertyUIFactory()
    {
        for(auto pair : m_factory)
        {
            delete pair.second;
        }

        delete m_propertyMgr;
    }
    
    IPropertyUI * PropertyUIFactory::createPropertyByName(const std::string & name)
    {
        PropertyFactory::iterator it = m_factory.find(name);
        if(it != m_factory.end())
        {
            return it->second->create(this);
        }

        return NULL;
    }
    
    void PropertyUIFactory::registerBasicProperty(const std::string & name, int type, SEL_CreatePropertyUI method)
    {
        BuiltinPropertyCreator *creator = new BuiltinPropertyCreator(method);
        auto ret = m_factory.insert(std::pair<std::string, PropertyCreator*>(name, creator));
        if(!ret.second)
        {
            LOG_ERROR("The property %s was covered.", name.c_str());

            delete ret.first->second;
            ret.first->second = creator;
        }

        m_nameToType[name] = type;
    }
    
    bool PropertyUIFactory::registerProertyTemplate(const std::string & filename)
    {
        rapidjson::Document document;
        if(!openJsonFile(filename, document))
        {
            return false;
        }
        
        typedef std::pair<std::string, PropertyCreator*> CreatorPair;
        
        for (rapidjson::Value::MemberIterator it = document.MemberBegin(); 
             it != document.MemberEnd(); ++it)
        {
            PropertyTypedef * declare = new PropertyTypedef();
            if(!declare->loadTypedef(it->value))
            {
                delete declare;
                return false;
            }

            PropertyCreator *creator = new CombinedPropertyCreator(declare);
            
            auto ret = m_factory.insert(CreatorPair(it->name.GetString(), creator));
            if(!ret.second)
            {
                LOG_ERROR("The property %s was covered.", it->name.GetString());
                
                delete ret.first->second;
                ret.first->second = creator;
            }
        }
        
        return true;
    }

    IPropertyUI* PropertyUIFactory::createPropertyByDef(PropertyTypedef *declare)
    {
        IPropertyUI * root = createPropertyByName(declare->m_type);
        if(NULL == root)
        {
            LOG_ERROR("Failed to create property ui for type '%s'", declare->m_type.c_str());
            return NULL;
        }

        root->setPropertyName(QString(declare->m_key.c_str()));
        root->setToolTip(QString(declare->m_desc.c_str()));
//        root->setText(declare->m_name);
        
        if(!declare->m_attributes.empty())
        {
            for(PropertyTypedef::Attributes::iterator it = declare->m_attributes.begin();
                it != declare->m_attributes.end(); ++it)
            {
                root->setAttribute(it.key(), it.value());
            }
        }
        
        if(!declare->m_items.empty())
        {
            for(PropertyTypedef::Children::iterator it = declare->m_items.begin();
                it != declare->m_items.end(); ++it)
            {
                IPropertyUI *item = createPropertyByDef(*it);
                if(NULL == item)
                {
                    LOG_ERROR("Failed to create property value for '%s'", (*it)->m_key.c_str());
                    return NULL;
                }
                
                root->addSubProperty(item);
            }
        }

        return root;
    }
    
    int PropertyUIFactory::name2type(const std::string & name)
    {
        auto it = m_nameToType.find(name);
        if(it != m_nameToType.end())
        {
            return it->second;
        }
        return QVariant::Invalid;
    }
}
