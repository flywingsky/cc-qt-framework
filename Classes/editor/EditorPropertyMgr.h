//
//  EditorValueTypeMgr.h
//  Editor
//
//  Created by C218-pc on 15/7/9.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Editor__EditorValueTypeMgr__
#define __Editor__EditorValueTypeMgr__

#include <platform/CCCommon.h>
#include <rapidjson/document.h>
#include "Singleton.h"

#include <string>
#include <map>

namespace Editor
{
    class IPropertyUI;
    class PropertyTypedef;
    
    class PropertyNode
    {
    public:
        PropertyNode();
        ~PropertyNode();
        
        bool load(rapidjson::Value & config);

        const std::string & getName() const { return m_name; }
        PropertyNode* getParent(){ return m_parent; }
        IPropertyUI * getPropertyUI() { return m_propertyUI; }
        
    private:
        std::string         m_name;
        PropertyNode*       m_parent;
        PropertyTypedef*    m_valueRoot;
        IPropertyUI*        m_propertyUI;
    };
    
    class PropertyMgr : public Singleton<PropertyMgr>
    {
    public:
        PropertyMgr();
        ~PropertyMgr();
        
        bool loadPropertyFile(const std::string & filename);
        PropertyNode* findProperty(const std::string & name);
        
        const std::string & cppNameToUIName(const std::string & className);
        void registerUIName(const std::string & className, const std::string & uiName);
        
    private:
        bool registerProperty(rapidjson::Value & value);
        
        typedef std::map<std::string, PropertyNode*> PropertyMap;
        PropertyMap m_properties;
        
        typedef std::map<std::string, std::string> StringMap;
        StringMap   m_class2uiName;
    };
}

#endif /* defined(__Editor__EditorValueTypeMgr__) */
