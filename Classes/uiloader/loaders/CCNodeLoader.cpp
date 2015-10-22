//
//  CCNodeLoader.cpp
//  Clover
//
//  Created by C218-pc on 15/6/26.
//
//

#include "CCNodeLoader.h"
#include "../UIHelper.h"

cocos2d::Node * CCNodeLoader::createObject(rapidjson::Value & config)
{
    return cocos2d::Node::create();
}

bool CCNodeLoader::setProperty(cocos2d::Node *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties)
{
    if(name == "name")
    {
        if(value.IsString())
        {
            p->setName(value.GetString());
        }
    }
    else if(name == "tag")
    {
        if(value.IsInt())
        {
            p->setTag(value.GetInt());
        }
    }
    else if(name == "position")
    {
        cocos2d::Vec2 pt;
        if(helper::parseValue(value, pt))
        {
            p->setPosition(pt);
        }
    }
    else if(name == "size")
    {
        cocos2d::Size size;
        if(helper::parseValue(value, size))
        {
            p->setContentSize(size);
        }
    }
    else if(name == "scale")
    {
        cocos2d::Vec2 pt;
        if(helper::parseValue(value, pt))
        {
            p->setScaleX(pt.x);
            p->setScaleY(pt.y);
        }
    }
    else if(name == "skew")
    {
        cocos2d::Vec2 pt;
        if(helper::parseValue(value, pt))
        {
            p->setSkewX(pt.x);
            p->setSkewY(pt.y);
        }
    }
    else if(name == "rotation")
    {
        cocos2d::Vec2 pt;
        if(helper::parseValue(value, pt))
        {
            p->setRotationX(pt.x);
            p->setRotationY(pt.y);
        }
    }
    else if(name == "anchor")
    {
        cocos2d::Vec2 pt;
        if(helper::parseValue(value, pt))
        {
            p->setAnchorPoint(pt);
        }
    }
    else if(name == "visible")
    {
        if(value.IsBool())
        {
            p->setVisible(value.GetBool());
        }
    }
    else if(name == "ignoreAnchor")
    {
        if(value.IsBool())
        {
            p->ignoreAnchorPointForPosition(value.GetBool());
        }
    }
    else if(name == "order")
    {
        if(value.IsInt())
        {
            p->setZOrder(value.GetInt());
        }
    }
    else if(name == "skin" || name == "children" || name == "property")
    {
        //ignore.
    }
    else
    {
        return false;
    }
    
    return true;
}

