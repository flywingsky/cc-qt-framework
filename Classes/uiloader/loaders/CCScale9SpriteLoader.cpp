//
//  CCScale9SpriteLoader.cpp
//  Clover
//
//  Created by C218-pc on 15/7/6.
//
//

#include "CCScale9SpriteLoader.h"

#include <GUI/CCControlExtension/Scale9Sprite.h>

#include "UIPlatform.h"
#include "UIHelper.h"

USING_NS_CC_EXT;

cocos2d::Node * CCScale9SpriteLoader::createObject(rapidjson::Value & config)
{
    return Scale9Sprite::create();
}

bool CCScale9SpriteLoader::setProperty(cocos2d::Node *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties)
{
    Scale9Sprite *sprite = dynamic_cast<Scale9Sprite*>(p);
    CCAssert(sprite, "CCScale9SpriteLoader::setProperty");
    
    if(name == "image")
    {
        if(value.IsString())
        {
            cocos2d::SpriteFrame * frame = uilib::createCCSpriteFrame(value.GetString());
            if(frame != NULL)
            {
                sprite->initWithSpriteFrame(frame);
            }
        }
    }
    else if(name == "capInsets")
    {
        cocos2d::Rect rc;
        if(helper::parseValue(value, rc))
        {
            sprite->setCapInsets(rc);
        }
    }
    else if(name == "preferredSize")
    {
        cocos2d::Size size;
        if(helper::parseValue(value, size))
        {
            sprite->setPreferredSize(size);
        }
    }
    else if(name == "color")
    {
        cocos2d::Color3B cr;
        if(helper::parseValue(value, cr))
        {
            sprite->setColor(cr);
        }
    }
    else if(name == "opacity")
    {
        if(value.IsInt())
        {
            sprite->setOpacity(value.GetInt());
        }
    }
    else
    {
        return base_class::setProperty(p, name, value, properties);
    }
    return true;
}
