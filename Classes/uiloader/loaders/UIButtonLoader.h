//
//  UIButtonLoader.h
//  Clover
//
//  Created by C218-pc on 15/6/29.
//
//

#ifndef __Clover__ButtonLoader__
#define __Clover__ButtonLoader__

#include "UIWidgetLoader.h"

class UIButtonLoader : public UIWidgetLoader
{
public:
    DECLARE_LOADER(UIButtonLoader);
    
    virtual cocos2d::CCNode * createObject(rapidjson::Value & config);
    virtual bool setProperty(cocos2d::CCNode *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties);
    
    virtual void trimProperty(rapidjson::Value & property, rapidjson::Value::AllocatorType & allocator) CC_OVERRIDE;
    virtual bool hookPropertyChange(PropertyParam & param) CC_OVERRIDE;
};

#endif /* defined(__Clover__UIButtonLoader__) */
