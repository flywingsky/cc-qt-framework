//
//  UIImageViewLoader.cpp
//  Clover
//
//  Created by C218-pc on 15/7/6.
//
//

#include "UIImageViewLoader.h"
#include "UIImageView.h"
#include "UIHelper.h"

cocos2d::CCNode * UIImageViewLoader::createObject(rapidjson::Value & config)
{
    return uilib::ImageView::create();
}

bool UIImageViewLoader::setProperty(cocos2d::CCNode *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties)
{
    uilib::ImageView *image = dynamic_cast<uilib::ImageView*>(p);
    CCAssert(image, "UIImageViewLoader::setProperty");
    
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
    else if(name == "flipX")
    {
        if(value.IsBool())
        {
            image->setFlipX(value.GetBool());
        }
    }
    else if(name == "flipY")
    {
        if(value.IsBool())
        {
            image->setFlipY(value.GetBool());
        }
    }
    else
    {
        return base_class::setProperty(p, name, value, properties);
    }
    return true;
}

void UIImageViewLoader::trimProperty(rapidjson::Value & property, rapidjson::Value::AllocatorType & allocator)
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

bool UIImageViewLoader::hookPropertyChange(PropertyParam & param)
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
