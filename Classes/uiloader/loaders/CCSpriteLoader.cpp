//
//  CCSpriteLoader.cpp
//  Clover
//
//  Created by C218-pc on 15/6/26.
//
//

#include "CCSpriteLoader.h"

#include <2d/CCSprite.h>

#include "../UIHelper.h"

DECLARE_PROPERTY_SETTER(cocos2d::Sprite, image, setTexture, std::string)
DECLARE_PROPERTY_SETTER(cocos2d::Sprite, imageRect, setTextureRect, cocos2d::Rect)
DECLARE_PROPERTY_SETTER(cocos2d::Sprite, blend, setBlendFunc, cocos2d::BlendFunc)

static void node_set_color(cocos2d::Sprite *node, const rapidjson::Value &value)
{
     cocos2d::Color4B cr;
     value >> cr;
     node->setColor(cocos2d::Color3B(cr));
     node->setOpacity(cr.a);
}

static void node_set_flip(cocos2d::Sprite *node, const rapidjson::Value &value)
{
    if(value.IsArray() && value.Size() >= 2)
    {
        bool flipX, flipY;
        value[0u] >> flipX;
        value[1] >> flipY;

        node->setFlipX(flipX);
        node->setFlipY(flipY);
    }
}

CCSpriteLoader::CCSpriteLoader()
{
#ifdef BUILD_EDITOR
    REGISTER_PROPERTY_SETTER(image);
    REGISTER_PROPERTY_SETTER(imageRect);
#endif

    REGISTER_PROPERTY_SETTER(blend);
    REGISTER_PROPERTY_SETTER(color);
    REGISTER_PROPERTY_SETTER(flip);
}

cocos2d::Node * CCSpriteLoader::createObject(rapidjson::Value & config)
{
    rapidjson::Value &image = config["image"];
    if(image.IsString())
    {
        rapidjson::Value &jrect = config["imageRect"];
        if(jrect.IsArray())
        {
            cocos2d::Rect rc;
            jrect >> rc;

            return cocos2d::Sprite::create(image.GetString(), rc);
        }

        return cocos2d::Sprite::create(image.GetString());
    }
    return cocos2d::Sprite::create();
}
