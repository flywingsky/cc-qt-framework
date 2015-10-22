//
//  UITextFieldExtLoader.cpp
//  Editor
//
//  Created by C218-pc on 15/8/11.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "UITextFieldExtLoader.h"
#include "UITextFieldExt.h"
#include "UIHelper.h"

cocos2d::Node * UITextFieldExtLoader::createObject(rapidjson::Value & config)
{
    return uilib::TextFieldExt::create();
}

bool UITextFieldExtLoader::setProperty(cocos2d::Node *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties)
{
    uilib::TextFieldExt *text = dynamic_cast<uilib::TextFieldExt*>(p);
    CCAssert(text, "UITextFieldExtLoader::setProperty");
    
    if(name == "placeholderText")
    {
        if(value.IsString())
        {
            text->setPlaceholderText(value.GetString());
        }
    }
    else if(name == "placeholderColor")
    {
        cocos2d::Color3B cr;
        if(helper::parseValue(value, cr))
        {
            text->setPlaceholderColor(cr);
        }
    }
    else
    {
        return base_class::setProperty(p, name, value, properties);
    }
    
    return true;
}
