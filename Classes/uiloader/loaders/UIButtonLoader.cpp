//
//  UIButtonLoader.cpp
//  Clover
//
//  Created by C218-pc on 15/6/29.
//
//

#include "UIButtonLoader.h"
#include "UIButton.h"
#include "UIHelper.h"

cocos2d::CCNode * UIButtonLoader::createObject(rapidjson::Value & config)
{
    return uilib::Button::create();
}

bool UIButtonLoader::setProperty(cocos2d::CCNode *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties)
{
    uilib::Button * btn = dynamic_cast<uilib::Button*>(p);
    CCAssert(btn, "UIButtonLoader::setProperty");
    
    if(name == "enable")
    {
        if(value.IsBool())
        {
            btn->setEnable(value.GetBool());
        }
    }
    else if(name == "text")
    {
        if(value.IsString())
        {
            btn->setText(value.GetString());
        }
    }
    else if(name == "fontName")
    {
        if(value.IsString())
        {
            btn->setFontName(value.GetString());
        }
    }
    else if(name == "fontSize")
    {
        if(value.IsInt())
        {
            btn->setFontSize(value.GetInt());
        }
    }
    else if(name == "fontColor")
    {
        cocos2d::ccColor3B cr;
        if(helper::parseValue(value, cr))
        {
            btn->setFontColor(cr);
        }
    }
    else if(name == "image")
    {
        std::string imageNormal, imagePressed, imageDisable;
        if(value.IsArray())
        {
            do
            {
                if(value.Size() < 1 || !value[0u].IsString()) break;
                imageNormal = value[0u].GetString();
                
                if(value.Size() < 2 || !value[1].IsString()) break;
                imagePressed = value[1].GetString();
                
                if(value.Size() < 3 || !value[2].IsString()) break;
                imageDisable = value[2].GetString();
                
            }while(0);
            btn->setImages(imageNormal, imagePressed, imageDisable);
        }
    }
    else if(name == "tileImage")
    {
        std::string imageTile, imageDisable;
        if(value.IsArray())
        {
            do
            {
                if(value.Size() < 1 || !value[0u].IsString()) break;
                imageTile = value[0u].GetString();
                
                if(value.Size() < 2 || !value[1].IsString()) break;
                imageDisable = value[1].GetString();
                
            }while(0);
            btn->setTileImage(imageTile, imageDisable);
        }
    }
    else if(name == "customSizeEnable")
    {
        if(value.IsBool())
        {
            btn->setCustomSizeEnable(value.GetBool());
        }
    }
    else
    {
        return base_class::setProperty(p, name, value, properties);
    }
    
    return true;
}

void UIButtonLoader::trimProperty(rapidjson::Value & property, rapidjson::Value::AllocatorType & allocator)
{
    rapidjson::Value *jvalue = &property["customSizeEnable"];
    
    if(!jvalue->IsBool() || !jvalue->GetBool())
    {
        property.RemoveMemberStable("size");
        property.RemoveMemberStable("percentSize");
        property.RemoveMemberStable("sizeType");
    }
    
    base_class::trimProperty(property, allocator);
}

bool UIButtonLoader::hookPropertyChange(PropertyParam & param)
{
    if(param.name == "size" || param.name == "percentSize" || param.name == "sizeType")
    {
        rapidjson::Value & jvalue = param.properties["customSizeEnable"];
        if(!jvalue.IsBool() || !jvalue.GetBool())
        {
            return false;
        }
    }
    
    return base_class::hookPropertyChange(param);
}
