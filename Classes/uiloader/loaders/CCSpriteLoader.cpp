//
//  CCSpriteLoader.cpp
//  Clover
//
//  Created by C218-pc on 15/6/26.
//
//

#include "CCSpriteLoader.h"

#include <sprite_nodes/CCSprite.h>
#include <textures/CCTextureCache.h>

#include "UIPlatform.h"
#include "UIHelper.h"

cocos2d::CCNode * CCSpriteLoader::createObject(rapidjson::Value & config)
{
    return cocos2d::CCSprite::create();
}

bool CCSpriteLoader::setProperty(cocos2d::CCNode *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties)
{
    cocos2d::CCSprite *sp = dynamic_cast<cocos2d::CCSprite*>(p);
    CCAssert(sp, "CCSpriteLoader::setProperty");
    
    if(name == "color")
    {
        cocos2d::ccColor3B cr;
        if(helper::parseValue(value, cr))
        {
            sp->setColor(cr);
        }
    }
    else if(name == "opacity")
    {
        if(value.IsInt())
        {
            sp->setOpacity(value.GetInt());
        }
    }
    else if(name == "image")
    {
        if(value.IsString())
        {
            cocos2d::CCTexture2D * texture = uilib::loadTexture(value.GetString());
            sp->setTexture(texture);
            
            if(texture != NULL)
            {
                rapidjson::Value & rect = properties["imageRect"];
                if(!rect.IsArray())
                {
                    // use default texture rect.
                    cocos2d::CCRect rc(0, 0, texture->getContentSize().width, texture->getContentSize().height);
                    sp->setTextureRect(rc);
                }
            }
        }
    }
    else if(name == "imageRect")
    {
        cocos2d::CCRect rc;
        if(helper::parseValue(value, rc))
        {
            sp->setTextureRect(rc);
        }
    }
    else if(name == "blend")
    {
        int blends[2];
        if(helper::parseNumberArray(value, blends, 2))
        {
            cocos2d::ccBlendFunc func = {blends[0], blends[1]};
            sp->setBlendFunc(func);
        }
    }
    else if(name == "flip")
    {
        if(value.IsArray() && value.Size() == 2)
        {
            if(value[0u].IsBool()) sp->setFlipX(value[0u].GetBool());
            if(value[1].IsBool()) sp->setFlipY(value[1].GetBool());
        }
    }
    else
    {
        return base_class::setProperty(p, name, value, properties);
    }
    
    return true;
}
