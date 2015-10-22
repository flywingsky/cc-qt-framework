//
//  CCLabelLoader.cpp
//  Clover
//
//  Created by C218-pc on 15/6/26.
//
//

#include "CCLabelLoader.h"
#include "UIHelper.h"

#include <label_nodes/CCLabelTTF.h>

cocos2d::Node * CCLabelLoader::createObject(rapidjson::Value & config)
{
    return cocos2d::LabelTTF::create();
}

bool CCLabelLoader::setProperty(cocos2d::Node *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties)
{
    cocos2d::LabelTTF * label = dynamic_cast<cocos2d::LabelTTF*>(p);
    CCAssert(label, "CCLabelLoader::setProperty");
    
    if(name == "text")
    {
        if(value.IsString())
        {
            label->setString(value.GetString());
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
        if(value.IsInt())
        {
            label->setFontSize(value.GetInt());
        }
    }
    else if(name == "fontColor")
    {
        cocos2d::Color3B cr;
        if(helper::parseValue(value, cr))
        {
            label->setFontFillColor(cr);
        }
    }
    else if(name == "dimension")
    {
        cocos2d::Size size;
        if(helper::parseValue(value, size))
        {
            label->setDimensions(size);
        }
    }
    else if(name == "horizontalAlignment")
    {
        if(value.IsInt())
        {
            label->setHorizontalAlignment((cocos2d::TextAlignment)value.GetInt());
        }
    }
    else if(name == "verticalAlignment")
    {
        if(value.IsInt())
        {
            label->setVerticalAlignment((cocos2d::VerticalTextAlignment)value.GetInt());
        }
    }
    else
    {
        return base_class::setProperty(p, name, value, properties);
    }
    
    return true;
}
