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
            SEL_CreatePropertyItem method_;
        public:
            CustomPropertyCreator(SEL_CreatePropertyItem method)
                : method_(method)
            {}

            virtual IPropertyItem* create(PropertyItemFactory *factory) override
            {
                return method_(factory->getPropertyMgr());
            }
        };

        class CombinedPropertyCreator : public PropertyCreator
        {
            PropertyItemType*    declare_;
        public:
            CombinedPropertyCreator(PropertyItemType *declare)
                : declare_(declare)
            {}

            ~CombinedPropertyCreator()
            {
                delete declare_;
            }

            virtual IPropertyItem* create(PropertyItemFactory *factory) override
            {
                return factory->createPropertyByDef(declare_);
            }
        };
    }

    PropertyItemType::PropertyItemType()
    {
    }
    
    PropertyItemType::~PropertyItemType()
    {
        for(std::vector<PropertyItemType*>::iterator it = items_.begin();
            it != items_.end(); ++it)
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
            child->key_ = it->name.GetString();
            
            if(!child->loadValue(it->value))
            {
                delete child;
                return false;
            }
            this->items_.push_back(child);
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
        type_ = value->GetString();
        
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
                
                items_.push_back(item);
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
                attributes_.insert(qkey, qvalue);
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
        type_ = value->GetString();

        value = &config[1];
        if(!value->IsString())
        {
            LOG_ERROR("Invalid item type, #1 must be a string type.");
            return false;
        }
        name_ = value->GetString();
        
        if(config.Size() > 2)
        {
            value = &config[2];
            if(value->IsString())
            {
                desc_ = value->GetString();
            }
        }
        
        return true;
    }
    
    //////////////////////////////////////////////////////////////////
    ///
    //////////////////////////////////////////////////////////////////
    PropertyItemFactory::PropertyItemFactory()
        : propertyMgr_(new QtVariantPropertyManager())
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
        for(auto pair : factory_)
        {
            delete pair.second;
        }

        delete propertyMgr_;
    }
    
    IPropertyItem * PropertyItemFactory::createPropertyByName(const std::string & name)
    {
        PropertyFactory::iterator it = factory_.find(name);
        if(it != factory_.end())
        {
            return it->second->create(this);
        }

        return NULL;
    }
    
    void PropertyItemFactory::registerPropertyCreator(const std::string & name, int type, PropertyCreator *creator)
    {
        auto ret = factory_.insert(std::pair<std::string, PropertyCreator*>(name, creator));
        if(!ret.second)
        {
            LOG_ERROR("The property %s was covered.", name.c_str());

            delete ret.first->second;
            ret.first->second = creator;
        }

        if(type != QVariant::Invalid)
        {
            nameToType_[name] = type;
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
            
            auto ret = factory_.insert(CreatorPair(it->name.GetString(), creator));
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
        IPropertyItem * root = createPropertyByName(declare->type_);
        if(NULL == root)
        {
            LOG_ERROR("Failed to create property ui for type '%s'", declare->type_.c_str());
            return NULL;
        }

        root->setPropertyName(QString(declare->key_.c_str()));
        root->setToolTip(QString(declare->desc_.c_str()));
//        root->setText(declare->name_);
        
        if(!declare->attributes_.empty())
        {
            for(PropertyItemType::Attributes::iterator it = declare->attributes_.begin();
                it != declare->attributes_.end(); ++it)
            {
                root->setAttribute(it.key(), it.value());
            }
        }
        
        if(!declare->items_.empty())
        {
            for(PropertyItemType::Children::iterator it = declare->items_.begin();
                it != declare->items_.end(); ++it)
            {
                IPropertyItem *item = createPropertyByDef(*it);
                if(NULL == item)
                {
                    LOG_ERROR("Failed to create property value for '%s'", (*it)->key_.c_str());
                    return NULL;
                }
                
                root->addSubProperty(item);
            }
        }

        return root;
    }
    
    int PropertyItemFactory::name2type(const std::string & name)
    {
        auto it = nameToType_.find(name);
        if(it != nameToType_.end())
        {
            return it->second;
        }
        return QVariant::Invalid;
    }
}
