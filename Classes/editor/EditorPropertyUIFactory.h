//
//  EditorPropertyUIFactory.h
//  Editor
//
//  Created by C218-pc on 15/7/20.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Editor__EditorPropertyValue__
#define __Editor__EditorPropertyValue__

#include "EditorValue.h"
#include "Singleton.h"

#include <rapidjson/document.h>

#include <string>
#include <vector>
#include <map>

namespace Editor
{
    class IPropertyUI;
    typedef IPropertyUI * (*SEL_CreatePropertyUI)();
    
    struct PropertyTypedef
    {
        PropertyTypedef();
        ~PropertyTypedef();
        
        bool loadChildren(const rapidjson::Value & config);
        bool loadTypedef(const rapidjson::Value & config);
        bool loadValue(const rapidjson::Value & config);
        
        std::string         m_type;
        std::string         m_skin;
        std::string         m_key;
        std::string         m_name;
        std::string         m_desc;
        TValue              m_defaultValue;
        TValue              m_extraArgs;
        
        std::vector<PropertyTypedef*> m_items;
    };
    
    class PropertyUIFactory : public Singleton<PropertyUIFactory>
    {
    public:
        PropertyUIFactory();
        ~PropertyUIFactory();
        
        IPropertyUI* createProperty(const std::string & name);
        IPropertyUI* createPropertyByType(PropertyTypedef * tp);
        
        void registerBasicProperty(const std::string & name, SEL_CreatePropertyUI method);
        bool registerProertyTemplate(const std::string & filename);
        
    private:
        IPropertyUI* createBasicProperty(const std::string & name);
        IPropertyUI* createCombinedProperty(const std::string & name);
        
        typedef std::map<std::string, SEL_CreatePropertyUI> PropertyFactory;
        PropertyFactory     m_factory;
        
        typedef std::map<std::string, PropertyTypedef*> PropertyTypedefMap;
        PropertyTypedefMap  m_declares;
    };
    
} // end namespace Editor

#endif /* defined(__Editor__EditorPropertyValue__) */
