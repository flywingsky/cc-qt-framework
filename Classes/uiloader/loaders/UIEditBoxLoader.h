//
//  UIEditBoxLoader.h
//  Editor
//
//  Created by C218-pc on 15/7/15.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Editor__UIEditBoxLoader__
#define __Editor__UIEditBoxLoader__

#include "UIWidgetLoader.h"

class UIEditBoxLoader : public UIWidgetLoader
{
public:
    DECLARE_LOADER(UIEditBoxLoader);
    
    virtual cocos2d::CCNode * createObject(rapidjson::Value & config);
    virtual bool setProperty(cocos2d::CCNode *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties);
};


#endif /* defined(__Editor__UIEditBoxLoader__) */
