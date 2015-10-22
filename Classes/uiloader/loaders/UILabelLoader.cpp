//
//  UILabelLoader.cpp
//  Clover
//
//  Created by C218-pc on 15/7/7.
//
//

#include "UILabelLoader.h"
#include "UILabel.h"
#include "UIHelper.h"

cocos2d::CCNode * UILabelLoader::createObject(rapidjson::Value & config)
{
    return uilib::Label::create();
}

bool UILabelLoader::setProperty(cocos2d::CCNode *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties)
{
    uilib::Label *label = dynamic_cast<uilib::Label*>(p);
    
    if(name == "text")
    {
        if(value.IsString())
        {
            label->setText(value.GetString());
        }
    }
    else if(name == "fontName")
    {
        if(value.IsString())
        {
            label->setFontName(value.GetString());
        }
    }
    else if(name == "fontSize")
    {
        if(value.IsNumber())
        {
            label->setFontSize(value.GetDouble());
        }
    }
    else if(name == "fontColor")
    {
        cocos2d::ccColor3B cr;
        if(helper::parseValue(value, cr))
        {
            label->setFontColor(cr);
        }
    }else if(name == "customSizeEnable")
    {
        if(value.IsBool())
        {
            label->setCustomSizeEnable(value.GetBool());
        }
    }
    else if(name == "horizontalAlignment")
    {
        if(value.IsInt())
        {
            label->setHorizontalAlignment((cocos2d::CCTextAlignment)value.GetInt());
        }
    }
    else if(name == "verticalAlignment")
    {
        if(value.IsInt())
        {
            label->setVerticalAlignment((cocos2d::CCVerticalTextAlignment)value.GetInt());
        }
    }
    else
    {
        return base_class::setProperty(p, name, value, properties);
    }
    
    return true;
}
