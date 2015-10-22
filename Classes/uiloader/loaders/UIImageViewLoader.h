//
//  UIImageViewLoader.h
//  Clover
//
//  Created by C218-pc on 15/7/6.
//
//

#ifndef __Clover__ImageViewLoader__
#define __Clover__ImageViewLoader__

#include "UIWidgetLoader.h"

class UIImageViewLoader : public UIWidgetLoader
{
public:
    DECLARE_LOADER(UIImageViewLoader);
    
    virtual cocos2d::CCNode * createObject(rapidjson::Value & config);
    virtual bool setProperty(cocos2d::CCNode *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties);
    
    virtual void trimProperty(rapidjson::Value & property, rapidjson::Value::AllocatorType & allocator) CC_OVERRIDE;
    virtual bool hookPropertyChange(PropertyParam & param) CC_OVERRIDE;
};

#endif /* defined(__Clover__UIImageViewLoader__) */
