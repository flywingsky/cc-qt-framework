//
//  UIFrameViewLoader.cpp
//  Editor
//
//  Created by C218-pc on 15/7/16.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "UIFrameViewLoader.h"
#include "UIFrameView.h"
#include "UIHelper.h"

cocos2d::CCNode * UIFrameViewLoader::createObject(rapidjson::Value & config)
{
    return uilib::FrameView::create();
}

bool UIFrameViewLoader::setProperty(cocos2d::CCNode *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties)
{
    uilib::FrameView *image = dynamic_cast<uilib::FrameView*>(p);
    CCAssert(image, "UIFrameViewLoader::setProperty");
    
    if (name == "image")
    {
        if(value.IsString())
        {
            image->setImage(value.GetString());
        }
    }
    else if(name == "customSizeEnable")
    {
        if(value.IsBool())
        {
            image->setCustomSizeEnable(value.GetBool());
        }
    }
    else if(name == "capInsets")
    {
        cocos2d::CCRect rc;
        if(helper::parseValue(value, rc))
        {
            image->setCapInsets(rc);
        }
    }
    else if(name == "color")
    {
        cocos2d::ccColor3B cr;
        if(helper::parseValue(value, cr))
        {
            image->setColor(cr);
        }
    }
    else if(name == "opacity")
    {
        if(value.IsInt())
        {
            image->setOpacity(value.GetInt());
        }
    }
    else
    {
        return base_class::setProperty(p, name, value, properties);
    }
    return true;
}

void UIFrameViewLoader::trimProperty(rapidjson::Value & property, rapidjson::Value::AllocatorType & allocator)
{
    rapidjson::Value *jvalue = &property["customSizeEnable"];
    
    if(jvalue->IsBool() && !jvalue->GetBool())
    {
        property.RemoveMemberStable("size");
        property.RemoveMemberStable("percentSize");
        property.RemoveMemberStable("sizeType");
    }
    
    base_class::trimProperty(property, allocator);
}
