//
//  UILayoutLoader.h
//  Clover
//
//  Created by C218-pc on 15/6/30.
//
//

#ifndef __Clover__UILayoutLoader__
#define __Clover__UILayoutLoader__

#include "UIWidgetLoader.h"

class UILayoutLoader : public UIWidgetLoader
{
public:
    DECLARE_LOADER(UILayoutLoader);
    
    virtual cocos2d::CCNode * createObject(rapidjson::Value & config);
    virtual bool setProperty(cocos2d::CCNode *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties);
    
    virtual bool upgradeProperty(rapidjson::Value & property, rapidjson::Value::AllocatorType & allocator);
};

#endif /* defined(__Clover__UILayoutLoader__) */
