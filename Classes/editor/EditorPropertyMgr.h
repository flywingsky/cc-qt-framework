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

class QtVariantProperty;

namespace Editor
{
    typedef QtVariantProperty IPropertyItem;
    class PropertyItemType;
    
    class PropertyTreeNode
    {
    public:
        PropertyTreeNode();
        ~PropertyTreeNode();
        
        bool load(rapidjson::Value & config);

        const std::string & getName() const{ return m_name; }
        PropertyTreeNode* getParent(){ return m_parent; }
        IPropertyItem * getPropertyItem(){ return m_propertyUI; }
        
    private:
        std::string         m_name;
        PropertyTreeNode*   m_parent;
        PropertyItemType*    m_valueRoot;
        IPropertyItem*        m_propertyUI;
    };
    
    class PropertyTreeMgr : public Singleton<PropertyTreeMgr>
    {
    public:
        PropertyTreeMgr();
        ~PropertyTreeMgr();
        
        bool loadPropertyFile(const std::string & filename);
        PropertyTreeNode* findProperty(const std::string & name);
        
        const std::string & cppNameToUIName(const std::string & className);
        void registerUIName(const std::string & className, const std::string & uiName);
        
    private:
        bool registerProperty(rapidjson::Value & value);
        
        typedef std::map<std::string, PropertyTreeNode*> PropertyMap;
        PropertyMap m_properties;
        
        typedef std::map<std::string, std::string> StringMap;
        StringMap   m_class2uiName;
    };
}

#endif /* defined(__Editor__EditorValueTypeMgr__) */
