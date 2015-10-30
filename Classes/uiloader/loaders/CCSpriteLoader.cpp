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

bool CCSpriteLoader::setProperty(PropertyParam &pp)
{
    cocos2d::Sprite *sp = dynamic_cast<cocos2d::Sprite*>(pp.node);
    CCAssert(sp, "CCSpriteLoader::setProperty");
    
    if(pp.name == "color")
    {
        cocos2d::Color4B cr;
        if(helper::parseValue(pp.value, cr))
        {
            sp->setColor(cocos2d::Color3B(cr));
            sp->setOpacity(cr.a);
        }
    }
    else if(pp.name == "image")
    {
        if(pp.value.IsString())
        {
            sp->setTexture(pp.value.GetString());
        }
    }
    else if(pp.name == "imageRect")
    {
        cocos2d::Rect rc;
        if(helper::parseValue(pp.value, rc))
        {
            sp->setTextureRect(rc);
        }
    }
    else if(pp.name == "blend")
    {
        int blends[2];
        if(helper::parseNumberArray(pp.value, blends, 2))
        {
            cocos2d::BlendFunc func = {(GLenum)blends[0], (GLenum)blends[1]};
            sp->setBlendFunc(func);
        }
    }
    else if(pp.name == "flip")
    {
        if(pp.value.IsArray() && pp.value.Size() == 2)
        {
            if(pp.value[0u].IsBool()) sp->setFlipX(pp.value[0u].GetBool());
            if(pp.value[1].IsBool()) sp->setFlipY(pp.value[1].GetBool());
        }
    }
    else
    {
        return base_class::setProperty(pp);
    }
    
    return true;
}
