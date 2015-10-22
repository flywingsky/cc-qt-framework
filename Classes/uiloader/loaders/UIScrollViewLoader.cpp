//
//  UIScrollViewLoader.cpp
//  Clover
//
//  Created by C218-pc on 15/7/2.
//
//

#include "UIScrollViewLoader.h"

#include "UIScrollView.h"

cocos2d::CCNode * UIScrollViewLoader::createObject(rapidjson::Value & config)
{
    return uilib::ScrollView::create();
}

bool UIScrollViewLoader::setProperty(cocos2d::CCNode *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties)
{
    uilib::ScrollView *scroll = dynamic_cast<uilib::ScrollView*>(p);
    CCAssert(scroll, "UIScrollViewLoader::setProperty");
    
    if(name == "direction")
    {
        if(value.IsInt())
        {
            scroll->setDirection((uilib::ScrollView::Direction)value.GetInt());
        }
    }
    else if(name == "bounceEnable")
    {
        if(value.IsBool())
        {
            scroll->setBoundceEnable(value.GetBool());
        }
    }
    else if(name == "bounceSpeed")
    {
        if(value.IsNumber())
        {
            scroll->setBounceSpeed(value.GetDouble());
        }
    }
    else if(name == "slideEnable")
    {
        if(value.IsBool())
        {
            scroll->setSlideEnable(value.GetBool());
        }
    }
    else if(name == "slideAccelerate")
    {
        if(value.IsNumber())
        {
            scroll->setSlideAccelerate(value.GetDouble());
        }
    }
    else
    {
        return base_class::setProperty(p, name, value, properties);
    }
    
    return true;
}
