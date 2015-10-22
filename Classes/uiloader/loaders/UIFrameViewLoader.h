//
//  UIFrameViewLoader.h
//  Editor
//
//  Created by C218-pc on 15/7/16.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Editor__UIFrameViewLoader__
#define __Editor__UIFrameViewLoader__

#include "UIWidgetLoader.h"

class UIFrameViewLoader : public UIWidgetLoader
{
public:
    DECLARE_LOADER(UIFrameViewLoader);
    
    virtual cocos2d::CCNode * createObject(rapidjson::Value & config);
    virtual bool setProperty(cocos2d::CCNode *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties);
    virtual void trimProperty(rapidjson::Value & property, rapidjson::Value::AllocatorType & allocator);
};


#endif /* defined(__Editor__UIFrameViewLoader__) */
