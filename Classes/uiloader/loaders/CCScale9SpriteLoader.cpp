//
//  CCScale9SpriteLoader.cpp
//  Clover
//
//  Created by C218-pc on 15/7/6.
//
//

#include "CCScale9SpriteLoader.h"

#include <GUI/CCControlExtension/CCScale9Sprite.h>

#include "UIPlatform.h"
#include "UIHelper.h"

USING_NS_CC_EXT;

cocos2d::CCNode * CCScale9SpriteLoader::createObject(rapidjson::Value & config)
{
    return CCScale9Sprite::create();
}

bool CCScale9SpriteLoader::setProperty(cocos2d::CCNode *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties)
{
    CCScale9Sprite *sprite = dynamic_cast<CCScale9Sprite*>(p);
    CCAssert(sprite, "CCScale9SpriteLoader::setProperty");
    
    if(name == "image")
    {
        if(value.IsString())
        {
            cocos2d::CCSpriteFrame * frame = uilib::createCCSpriteFrame(value.GetString());
            if(frame != NULL)
            {
                sprite->initWithSpriteFrame(frame);
            }
        }
    }
    else if(name == "capInsets")
    {
        cocos2d::CCRect rc;
        if(helper::parseValue(value, rc))
        {
            sprite->setCapInsets(rc);
        }
    }
    else if(name == "preferredSize")
    {
        cocos2d::CCSize size;
        if(helper::parseValue(value, size))
        {
            sprite->setPreferredSize(size);
        }
    }
    else if(name == "color")
    {
        cocos2d::ccColor3B cr;
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
