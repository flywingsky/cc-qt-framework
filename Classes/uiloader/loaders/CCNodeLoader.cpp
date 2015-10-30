//
//  CCNodeLoader.cpp
//  Clover
//
//  Created by C218-pc on 15/6/26.
//
//

#include "CCNodeLoader.h"
#include "../UIHelper.h"

cocos2d::Node* CCNodeLoader::createObject(rapidjson::Value & config)
{
    return cocos2d::Node::create();
}

bool CCNodeLoader::setProperty(PropertyParam &pp)
{
    if(pp.name == "name")
    {
        if(pp.value.IsString())
        {
            pp.node->setName(pp.value.GetString());
        }
    }
    else if(pp.name == "tag")
    {
        if(pp.value.IsInt())
        {
            pp.node->setTag(pp.value.GetInt());
        }
    }
    else if(pp.name == "position")
    {
        cocos2d::Vec2 pt;
        if(helper::parseValue(pp.value, pt))
        {
            pp.node->setPosition(pt);
        }
    }
    else if(pp.name == "size")
    {
        cocos2d::Size size;
        if(helper::parseValue(pp.value, size))
        {
            pp.node->setContentSize(size);
        }
    }
    else if(pp.name == "scale")
    {
        cocos2d::Vec2 pt;
        if(helper::parseValue(pp.value, pt))
        {
            pp.node->setScaleX(pt.x);
            pp.node->setScaleY(pt.y);
        }
    }
    else if(pp.name == "skew")
    {
        cocos2d::Vec2 pt;
        if(helper::parseValue(pp.value, pt))
        {
            pp.node->setSkewX(pt.x);
            pp.node->setSkewY(pt.y);
        }
    }
    else if(pp.name == "rotation")
    {
        cocos2d::Vec2 pt;
        if(helper::parseValue(pp.value, pt))
        {
            pp.node->setRotationX(pt.x);
            pp.node->setRotationY(pt.y);
        }
    }
    else if(pp.name == "anchor")
    {
        cocos2d::Vec2 pt;
        if(helper::parseValue(pp.value, pt))
        {
            pp.node->setAnchorPoint(pt);
        }
    }
    else if(pp.name == "visible")
    {
        if(pp.value.IsBool())
        {
            pp.node->setVisible(pp.value.GetBool());
        }
    }
    else if(pp.name == "ignoreAnchor")
    {
        if(pp.value.IsBool())
        {
            pp.node->ignoreAnchorPointForPosition(pp.value.GetBool());
        }
    }
    else if(pp.name == "order")
    {
        if(pp.value.IsInt())
        {
            pp.node->setZOrder(pp.value.GetInt());
        }
    }
    else
    {
        return false;
    }
    
    return true;
}

