//
//  EditorPropertyValue.cpp
//  Editor
//
//  Created by C218-pc on 15/7/20.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "EditorPropertyItem.h"
#include "EditorTools.h"
#include "LogTool.h"

#include "uiloader/UIHelper.h"

#include <qtvariantproperty.h>

DEFINE_LOG_COMPONENT(LOG_PRIORITY_DEBUG, "PropertyItemFactory");
IMPLEMENT_SINGLETON(Editor::PropertyItemFactory);

namespace Editor
{
    class PropertyCreator
    {
    public:
        virtual ~PropertyCreator(){}
        virtual IPropertyItem* create(PropertyItemFactory *factory) = 0;
    };

    namespace
    {
        bool parseAttributeValue(QVariant & qvalue, const rapidjson::Value & jvalue)
        {
            if(jvalue.IsArray() && jvalue.Size() == 2)
            {
                std::string type = jvalue[0u].GetString();
                int itype = PropertyItemFactory::instance()->name2type(type);

                json2tvalue(qvalue, jvalue[1], itype);
                return true;
            }
            return false;
        }

        class BuiltinPropertyCreator : public PropertyCreator
        {
            int     type_;
        public:
            BuiltinPropertyCreator(int type)
                : type_(type)
            {}

            virtual IPropertyItem* create(PropertyItemFactory *factory) override
            {
                return factory->getPropertyMgr()->addProperty(type_);
            }
        };

        class CustomPropertyCreator : public PropertyCreator
        {
            SEL_CreatePropertyItem m_method;
        public:
            CustomPropertyCreator(SEL_CreatePropertyItem method)
                : m_method(method)
            {}

            virtual IPropertyItem* create(PropertyItemFactory *factory) override
            {
                return m_method(factory->getPropertyMgr());
            }
        };

        class CombinedPropertyCreator : public PropertyCreator
        {
            PropertyItemType*    m_declare;
        public:
            CombinedPropertyCreator(PropertyItemType *declare)
                : m_declare(declare)
            {}

            ~CombinedPropertyCreator()
            {
                delete m_declare;
            }

            virtual IPropertyItem* create(PropertyItemFactory *factory) override
            {
                return factory->createPropertyByDef(m_declare);
            }
        };
    }

    PropertyItemType::PropertyItemType()
    {
    }
    
    PropertyItemType::~PropertyItemType()
    {
        for(std::vector<PropertyItemType*>::iterator it = m_items.begin();
            it != m_items.end(); ++it)
        {
            delete (*it);
        }
    }
    
    bool PropertyItemType::loadChildren(const rapidjson::Value &config)
    {
        for(rapidjson::Value::ConstMemberIterator it = config.MemberBegin();
            it != config.MemberEnd(); ++it)
        {
            PropertyItemType *child = new PropertyItemType();
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
    
    bool PropertyItemType::loadTypedef(const rapidjson::Value & config)
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
                PropertyItemType *item = new PropertyItemType();
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
    
    bool PropertyItemType::loadValue(const rapidjson::Value & config)
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
    PropertyItemFactory::PropertyItemFactory()
        : m_propertyMgr(new QtVariantPropertyManager())
    {
#define REG_PROPERTY(NAME, TYPE) \
    registerPropertyCreator(NAME, TYPE, new BuiltinPropertyCreator(TYPE));

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
        REG_PROPERTY("stringList", QVariant::StringList);
        REG_PROPERTY("enum", QtVariantPropertyManager::enumTypeId());
        REG_PROPERTY("class", QtVariantPropertyManager::groupTypeId());
        REG_PROPERTY("group", QtVariantPropertyManager::groupTypeId());

#undef REG_PROPERTY
    }
    
    PropertyItemFactory::~PropertyItemFactory()
    {
        for(auto pair : m_factory)
        {
            delete pair.second;
        }

        delete m_propertyMgr;
    }
    
    IPropertyItem * PropertyItemFactory::createPropertyByName(const std::string & name)
    {
        PropertyFactory::iterator it = m_factory.find(name);
        if(it != m_factory.end())
        {
            return it->second->create(this);
        }

        return NULL;
    }
    
    void PropertyItemFactory::registerPropertyCreator(const std::string & name, int type, PropertyCreator *creator)
    {
        auto ret = m_factory.insert(std::pair<std::string, PropertyCreator*>(name, creator));
        if(!ret.second)
        {
            LOG_ERROR("The property %s was covered.", name.c_str());

            delete ret.first->second;
            ret.first->second = creator;
        }

        if(type != QVariant::Invalid)
        {
            m_nameToType[name] = type;
        }
    }

    void PropertyItemFactory::registerCustomProperty(const std::string & name, int type, SEL_CreatePropertyItem method)
    {
        CustomPropertyCreator *creator = new CustomPropertyCreator(method);
        registerPropertyCreator(name, type, creator);
    }
    
    bool PropertyItemFactory::registerProertyTemplate(const std::string & filename)
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
            PropertyItemType * declare = new PropertyItemType();
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

    IPropertyItem* PropertyItemFactory::createPropertyByDef(PropertyItemType *declare)
    {
        IPropertyItem * root = createPropertyByName(declare->m_type);
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
            for(PropertyItemType::Attributes::iterator it = declare->m_attributes.begin();
                it != declare->m_attributes.end(); ++it)
            {
                root->setAttribute(it.key(), it.value());
            }
        }
        
        if(!declare->m_items.empty())
        {
            for(PropertyItemType::Children::iterator it = declare->m_items.begin();
                it != declare->m_items.end(); ++it)
            {
                IPropertyItem *item = createPropertyByDef(*it);
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
    
    int PropertyItemFactory::name2type(const std::string & name)
    {
        auto it = m_nameToType.find(name);
        if(it != m_nameToType.end())
        {
            return it->second;
        }
        return QVariant::Invalid;
    }
}
