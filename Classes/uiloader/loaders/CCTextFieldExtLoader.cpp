//
//  CCTextFieldExtLoader.cpp
//  Editor
//
//  Created by C218-pc on 15/8/11.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "CCTextFieldExtLoader.h"

#include "../CCTextFieldExt.h"
#include "../UIHelper.h"


cocos2d::CCNode * CCTextFieldExtLoader::createObject(rapidjson::Value & config)
{
    return cocos2d::CCTextFieldTTFExt::create();
}

bool CCTextFieldExtLoader::setProperty(cocos2d::CCNode *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties)
{
    cocos2d::CCTextFieldTTFExt *text = dynamic_cast<cocos2d::CCTextFieldTTFExt*>(p);
    CCAssert(text, "CCTextFieldExtLoader::setProperty");
    
    if(name == "placeholderText")
    {
        if(value.IsString())
        {
            text->setPlaceholderText(value.GetString());
        }
    }
    else if(name == "placeholderColor")
    {
        cocos2d::ccColor3B cr;
        if(helper::parseValue(value, cr))
        {
            text->setPlaceholderColor(cr);
        }
    }
    else if(name == "isPassword")
    {
        if(value.IsBool())
        {
            text->setPasswordEnable(value.GetBool());
        }
    }
    else
    {
        return base_class::setProperty(p, name, value, properties);
    }
    
    return true;
}
