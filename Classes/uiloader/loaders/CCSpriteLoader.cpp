//
//  CCSpriteLoader.cpp
//  Clover
//
//  Created by C218-pc on 15/6/26.
//
//

#include "CCSpriteLoader.h"

#include <2d/CCSprite.h>
#include <renderer/CCTextureCache.h>

#include "../UIHelper.h"

cocos2d::Node * CCSpriteLoader::createObject(rapidjson::Value & config)
{
    return cocos2d::Sprite::create();
}

bool CCSpriteLoader::setProperty(cocos2d::Node *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties)
{
    cocos2d::Sprite *sp = dynamic_cast<cocos2d::Sprite*>(p);
    CCAssert(sp, "CCSpriteLoader::setProperty");
    
    if(name == "color")
    {
        cocos2d::Color3B cr;
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
            sp->setTexture(value.GetString());
        }
    }
    else if(name == "imageRect")
    {
        cocos2d::Rect rc;
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
            cocos2d::BlendFunc func = {(GLenum)blends[0], (GLenum)blends[1]};
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
