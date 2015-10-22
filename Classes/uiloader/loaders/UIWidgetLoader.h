//
//  UIWidgetLoader.h
//  Clover
//
//  Created by C218-pc on 15/7/2.
//
//

#ifndef __Clover__UIWidgetLoader__
#define __Clover__UIWidgetLoader__

#include "CCNodeLoader.h"

class UIWidgetLoader : public CCNodeLoader
{
public:
    DECLARE_LOADER(UIWidgetLoader);
    
    virtual cocos2d::Node * createObject(rapidjson::Value & config);
    virtual bool setProperty(cocos2d::Node *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties);
    
    virtual bool upgradeProperty(rapidjson::Value & root, rapidjson::Value::AllocatorType & allocator);
    virtual void trimProperty(rapidjson::Value & property, rapidjson::Value::AllocatorType & allocator);
};

#endif /* defined(__Clover__UIWidgetLoader__) */
