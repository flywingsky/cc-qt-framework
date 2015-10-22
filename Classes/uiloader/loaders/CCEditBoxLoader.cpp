//
//  CCEditBoxLoader.cpp
//  Editor
//
//  Created by C218-pc on 15/7/15.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "CCEditBoxLoader.h"
#include "UIHelper.h"
#include "UIPlatform.h"

#include <GUI/CCEditBox/CCEditBox.h>
#include <GUI/CCControlExtension/CCScale9Sprite.h>

USING_NS_CC;
USING_NS_CC_EXT;

CCScale9Sprite * createScale9Sprite(const char * image)
{
    cocos2d::CCTexture2D * texture = uilib::loadTexture(image);
    if(NULL == texture)
    {
        return NULL;
    }
    
    cocos2d::CCRect rc(0, 0, texture->getContentSize().width, texture->getContentSize().height);
    
    cocos2d::CCSpriteFrame * frame = cocos2d::CCSpriteFrame::createWithTexture(texture, rc);
    
    return CCScale9Sprite::createWithSpriteFrame(frame);
}

cocos2d::CCNode * CCEditBoxLoader::createObject(rapidjson::Value & config)
{
    rapidjson::Value & image = config["image"];
    if(!image.IsString())
    {
        CCLOGERROR("CCEditBoxLoader: failed to find property 'image'");
        return NULL;
    }
    
    CCScale9Sprite *sprite = createScale9Sprite(image.GetString());
    if(NULL == sprite)
    {
        CCLOGERROR("CCEditBoxLoader: failed to create background sprite");
        return NULL;
    }
    
    CCSize size(200, 50);
    helper::parseValue(config["size"], size);
    
    return CCEditBox::create(size, sprite);
}

bool CCEditBoxLoader::setProperty(cocos2d::CCNode *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties)
{
    CCEditBox *edit = dynamic_cast<CCEditBox*>(p);
    CCAssert(edit, "CCEditBoxLoader::setProperty");
    
    if(name == "text")
    {
        if(value.IsString())
        {
            edit->setText(value.GetString());
        }
    }
    else if(name == "image")
    {
        if(value.IsString())
        {
            CCScale9Sprite *sprite = createScale9Sprite(value.GetString());
            if(sprite)
            {
                edit->setBackgroundSprite(sprite);
            }
        }
    }
    else if(name == "fontName")
    {
        if(value.IsString())
        {
            edit->setFontName(value.GetString());
        }
    }
    else if(name == "fontSize")
    {
        if(value.IsInt())
        {
            edit->setFontSize(value.GetInt());
        }
    }
    else if(name == "fontColor")
    {
        ccColor3B cr;
        if(helper::parseValue(value, cr))
        {
            edit->setFontColor(cr);
        }
    }
    else if(name == "placeholderText")
    {
        if(value.IsString())
        {
            edit->setPlaceHolder(value.GetString());
        }
    }
    else if(name == "placeholderFontName")
    {
        if(value.IsString())
        {
            edit->setPlaceholderFontName(value.GetString());
        }
    }
    else if(name == "placeholderFontSize")
    {
        if(value.IsInt())
        {
            edit->setPlaceholderFontSize(value.GetInt());
        }
    }
    else if(name == "placeholderFontColor")
    {
        cocos2d::ccColor3B cr;
        if(helper::parseValue(value, cr))
        {
            edit->setPlaceholderFontColor(cr);
        }
    }
    else if(name == "maxLength")
    {
        if(value.IsInt())
        {
            edit->setMaxLength(value.GetInt());
        }
    }
    else if(name == "inputMode")
    {
        if(value.IsInt())
        {
            edit->setInputMode(EditBoxInputMode(value.GetInt()));
        }
    }
    else if(name == "inputFlag")
    {
        if(value.IsInt())
        {
            edit->setInputFlag(EditBoxInputFlag(value.GetInt()));
        }
    }
    else if(name == "returnType")
    {
        if(value.IsInt())
        {
            edit->setReturnType(KeyboardReturnType(value.GetInt()));
        }
    }
    else
    {
        return base_class::setProperty(p, name, value, properties);
    }
    return true;
}
