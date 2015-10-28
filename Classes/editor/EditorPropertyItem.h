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
        
        std::string         type_;
        std::string         key_;
        std::string         name_;
        std::string         desc_;

        typedef QMap<QString, QVariant> Attributes;
        Attributes          attributes_;
        
        typedef std::vector<PropertyItemType*> Children;
        Children            items_;
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

        QtVariantPropertyManager* getPropertyMgr(){ return propertyMgr_; }

    private:
        typedef std::map<std::string, PropertyCreator*> PropertyFactory;
        PropertyFactory     factory_;

        typedef std::map<std::string, int> NameToType;
        NameToType          nameToType_;

        QtVariantPropertyManager*   propertyMgr_;
    };
    
} // end namespace Editor

#endif /* defined(__Editor__EditorPropertyValue__) */
