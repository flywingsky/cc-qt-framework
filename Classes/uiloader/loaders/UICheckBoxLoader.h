//
//  UICheckBox.h
//  Editor
//
//  Created by C218-pc on 15/7/25.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Editor__UICheckBoxLoader__
#define __Editor__UICheckBoxLoader__

#include "UIButtonLoader.h"

class UICheckBoxLoader : public UIButtonLoader
{
public:
    DECLARE_LOADER(UICheckBoxLoader);
    
    virtual cocos2d::Node * createObject(rapidjson::Value & config);
    virtual bool setProperty(cocos2d::Node *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties);
};

#endif /* defined(__Editor__UICheckBoxLoader__) */
