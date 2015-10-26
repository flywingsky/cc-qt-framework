//
//  EditorPropertyUIFactory.h
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
    typedef QtVariantProperty IPropertyUI;
    typedef IPropertyUI * (*SEL_CreatePropertyUI)(QtVariantPropertyManager * /*mgr*/);
    
    class PropertyTypedef
    {
    public:
        PropertyTypedef();
        ~PropertyTypedef();
        
        bool loadChildren(const rapidjson::Value & config);
        bool loadTypedef(const rapidjson::Value & config);
        bool loadValue(const rapidjson::Value & config);
        
        std::string         m_type;
        std::string         m_key;
        std::string         m_name;
        std::string         m_desc;

        typedef QMap<QString, QVariant> Attributes;
        Attributes          m_attributes;
        
        typedef std::vector<PropertyTypedef*> Children;
        Children            m_items;
    };
    
    class PropertyUIFactory : public Singleton<PropertyUIFactory>
    {
    public:
        PropertyUIFactory();
        ~PropertyUIFactory();
        
        IPropertyUI* createPropertyByName(const std::string & name);
        IPropertyUI* createPropertyByDef(PropertyTypedef * tp);
        
        void registerBasicProperty(const std::string & name, int type, SEL_CreatePropertyUI method);
        bool registerProertyTemplate(const std::string & filename);
        
        int name2type(const std::string & name);

    private:
        IPropertyUI* createBasicProperty(const std::string & name);
        IPropertyUI* createCombinedProperty(const std::string & name);
        
        typedef std::map<std::string, SEL_CreatePropertyUI> PropertyFactory;
        PropertyFactory     m_factory;
        
        typedef std::map<std::string, PropertyTypedef*> PropertyTypedefMap;
        PropertyTypedefMap  m_declares;

        typedef std::map<std::string, int> NameToType;
        NameToType          m_nameToType;

        QtVariantPropertyManager*   m_propertyMgr;
    };
    
} // end namespace Editor

#endif /* defined(__Editor__EditorPropertyValue__) */
