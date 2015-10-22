//
//  UIEditBoxLoader.cpp
//  Editor
//
//  Created by C218-pc on 15/7/15.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "UIEditBoxLoader.h"
#include "UIEditBox.h"

#include "UIHelper.h"

USING_NS_CC;

cocos2d::Node * UIEditBoxLoader::createObject(rapidjson::Value & config)
{
    return uilib::EditBox::create();
}

bool UIEditBoxLoader::setProperty(cocos2d::Node *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties)
{
    uilib::EditBox *edit = dynamic_cast<uilib::EditBox*>(p);
    CCAssert(edit, "UIEditBoxLoader::setProperty");
    
    if(name == "text")
    {
        if(value.IsString())
        {
            edit->setText(value.GetString());
        }
    }
    else if(name == "image")
    {
        if(value.IsString())
        {
            edit->setBackgroundImage(value.GetString());
        }
    }
    else if(name == "fontName")
    {
        if(value.IsString())
        {
            edit->setFontName(value.GetString());
        }
    }
    else if(name == "fontSize")
    {
        if(value.IsInt())
        {
            edit->setFontSize(value.GetInt());
        }
    }
    else if(name == "fontColor")
    {
        Color3B cr;
        if(helper::parseValue(value, cr))
        {
            edit->setFontColor(cr);
        }
    }
    else if(name == "placeholderText")
    {
        if(value.IsString())
        {
            edit->setPlaceholderText(value.GetString());
        }
    }
    else if(name == "placeholderFontName")
    {
        if(value.IsString())
        {
            edit->setPlaceholderFontName(value.GetString());
        }
    }
    else if(name == "placeholderFontSize")
    {
        if(value.IsInt())
        {
            edit->setPlaceholderFontSize(value.GetInt());
        }
    }
    else if(name == "placeholderFontColor")
    {
        cocos2d::Color3B cr;
        if(helper::parseValue(value, cr))
        {
            edit->setPlaceholderFontColor(cr);
        }
    }
    else if(name == "maxLength")
    {
        if(value.IsInt())
        {
            edit->setMaxLength(value.GetInt());
        }
    }
    else if(name == "inputMode")
    {
        if(value.IsInt())
        {
            edit->setInputMode(cocos2d::extension::EditBoxInputMode(value.GetInt()));
        }
    }
    else if(name == "inputFlag")
    {
        if(value.IsInt())
        {
            edit->setInputFlag(cocos2d::extension::EditBoxInputFlag(value.GetInt()));
        }
    }
    else if(name == "returnType")
    {
        if(value.IsInt())
        {
            edit->setReturnType(cocos2d::extension::KeyboardReturnType(value.GetInt()));
        }
    }
    else
    {
        return base_class::setProperty(p, name, value, properties);
    }
    return true;
}
