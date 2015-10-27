//
//  EditorPropertyItemFactory.h
//  Editor
//
//  Created by C218-pc on 15/7/20.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Editor__EditorPropertyValue__
#define __Editor__EditorPropertyValue__

#include "Singleton.h"

#include <rapidjson/document.h>

#include <string>
#include <vector>
#include <map>

#include <QVariant>

class QtVariantPropertyManager;
class QtVariantProperty;

namespace Editor
{
    class PropertyCreator;

    typedef QtVariantProperty IPropertyItem;
    typedef IPropertyItem* (*SEL_CreatePropertyItem)(QtVariantPropertyManager * /*mgr*/);
    
    class PropertyItemType
    {
    public:
        PropertyItemType();
        ~PropertyItemType();
        
        bool loadChildren(const rapidjson::Value & config);
        bool loadTypedef(const rapidjson::Value & config);
        bool loadValue(const rapidjson::Value & config);
        
        std::string         m_type;
        std::string         m_key;
        std::string         m_name;
        std::string         m_desc;

        typedef QMap<QString, QVariant> Attributes;
        Attributes          m_attributes;
        
        typedef std::vector<PropertyItemType*> Children;
        Children            m_items;
    };


    class PropertyItemFactory : public Singleton<PropertyItemFactory>
    {
    public:
        PropertyItemFactory();
        ~PropertyItemFactory();
        
        IPropertyItem* createPropertyByName(const std::string & name);
        IPropertyItem* createPropertyByDef(PropertyItemType * tp);
        
        void registerPropertyCreator(const std::string & name, int type, PropertyCreator *creator);
        void registerCustomProperty(const std::string & name, int type, SEL_CreatePropertyItem method);
        bool registerProertyTemplate(const std::string & filename);
        
        int name2type(const std::string & name);

        QtVariantPropertyManager* getPropertyMgr(){ return m_propertyMgr; }

    private:
        typedef std::map<std::string, PropertyCreator*> PropertyFactory;
        PropertyFactory     m_factory;

        typedef std::map<std::string, int> NameToType;
        NameToType          m_nameToType;

        QtVariantPropertyManager*   m_propertyMgr;
    };
    
} // end namespace Editor

#endif /* defined(__Editor__EditorPropertyValue__) */
