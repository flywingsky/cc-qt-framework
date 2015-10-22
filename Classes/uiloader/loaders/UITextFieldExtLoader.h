//
//  UITextFieldExtLoader.h
//  Editor
//
//  Created by C218-pc on 15/8/11.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Editor__UITextFieldExtLoader__
#define __Editor__UITextFieldExtLoader__

#include "UILabelLoader.h"

class UITextFieldExtLoader : public UILabelLoader
{
public:
    DECLARE_LOADER(UITextFieldExtLoader);
    
    virtual cocos2d::CCNode * createObject(rapidjson::Value & config);
    virtual bool setProperty(cocos2d::CCNode *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties);
};


#endif /* defined(__Editor__UITextFieldExtLoader__) */
