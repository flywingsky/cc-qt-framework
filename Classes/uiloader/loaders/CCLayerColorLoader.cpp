//
//  CCLayerColorLoader.cpp
//  Editor
//
//  Created by C218-pc on 15/7/31.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "CCLayerColorLoader.h"
#include "../UIHelper.h"

#include <layers_scenes_transitions_nodes/CCLayer.h>

cocos2d::Node * CCLayerColorLoader::createObject(rapidjson::Value & config)
{
    return cocos2d::LayerColor::create(cocos2d::ccc4(255, 255, 255, 255), 100, 100);
}

bool CCLayerColorLoader::setProperty(cocos2d::Node *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties)
{
    cocos2d::LayerColor * layer = dynamic_cast<cocos2d::LayerColor*>(p);
    CCAssert(layer != NULL, "CCLayerColorLoader::setProperty");
    
    if(name == "color")
    {
        cocos2d::Color3B cr;
        if(helper::parseValue(value, cr))
        {
            layer->setColor(cr);
        }
    }
    else if(name == "opacity")
    {
        if(value.IsInt())
        {
            layer->setOpacity(value.GetInt());
        }
    }
    else
    {
        base_class::setProperty(p, name, value, properties);
    }
    return true;
}
