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

        const std::string & getName() const{ return name_; }
        PropertyTreeNode* getParent(){ return parent_; }
        IPropertyItem * getPropertyItem(){ return propertyUI_; }

        IPropertyItem* findPropertyItem(const std::string &name);
        
    private:
        std::string         name_;
        PropertyTreeNode*   parent_;
        PropertyItemType*   valueRoot_;
        IPropertyItem*      propertyUI_;
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
        PropertyMap properties_;
        
        typedef std::map<std::string, std::string> StringMap;
        StringMap   class2uiName_;
    };
}

#endif /* defined(__Editor__EditorValueTypeMgr__) */
