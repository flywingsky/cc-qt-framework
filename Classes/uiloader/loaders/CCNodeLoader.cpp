//
//  CCNodeLoader.cpp
//  Clover
//
//  Created by C218-pc on 15/6/26.
//
//

#include "CCNodeLoader.h"
#include "../UIHelper.h"

#define DECLARE_SETTER(PROPERTY_NAME, METHOD_NAME, VALUE_TYPE) \
DECLARE_PROPERTY_SETTER(cocos2d::Node, PROPERTY_NAME, METHOD_NAME, VALUE_TYPE)

DECLARE_SETTER(name, setName, std::string)
DECLARE_SETTER(tag, setTag, int)
DECLARE_SETTER(order, setZOrder, int)
DECLARE_SETTER(position, setPosition3D, cocos2d::Vec3)
DECLARE_SETTER(rotation, setRotation3D, cocos2d::Vec3)
DECLARE_SETTER(size, setContentSize, cocos2d::Size)
DECLARE_SETTER(anchor, setAnchorPoint, cocos2d::Vec2)
DECLARE_SETTER(ignoreAnchor, ignoreAnchorPointForPosition, bool)
DECLARE_SETTER(visible, setVisible, bool)

#undef DECLARE_SETTER

static void node_set_skew(cocos2d::Node *node, const rapidjson::Value &value)
{
    cocos2d::Vec2 v;
    value >> v;
    node->setSkewX(v.x);
    node->setSkewY(v.y);
}

static void node_set_scale(cocos2d::Node *node, const rapidjson::Value &value)
{
    cocos2d::Vec3 v;
    value >> v;
    node->setScaleX(v.x);
    node->setScaleY(v.y);
    node->setScaleZ(v.z);
}

CCNodeLoader::CCNodeLoader()
{
    REGISTER_PROPERTY_SETTER(name);
    REGISTER_PROPERTY_SETTER(tag);
    REGISTER_PROPERTY_SETTER(order);
    REGISTER_PROPERTY_SETTER(position);
    REGISTER_PROPERTY_SETTER(scale);
    REGISTER_PROPERTY_SETTER(rotation);
    REGISTER_PROPERTY_SETTER(size);
    REGISTER_PROPERTY_SETTER(anchor);
    REGISTER_PROPERTY_SETTER(ignoreAnchor);
    REGISTER_PROPERTY_SETTER(visible);
    REGISTER_PROPERTY_SETTER(skew);
}

cocos2d::Node* CCNodeLoader::createObject(rapidjson::Value & config)
{
    return cocos2d::Node::create();
}
