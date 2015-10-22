//
//  CCLayerLoader.cpp
//  Clover
//
//  Created by C218-pc on 15/6/29.
//
//

#include "CCLayerLoader.h"
#include <layers_scenes_transitions_nodes/CCLayer.h>


cocos2d::CCNode * CCLayerLoader::createObject(rapidjson::Value & config)
{
    return cocos2d::CCLayer::create();
}

bool CCLayerLoader::setProperty(cocos2d::CCNode *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties)
{
    cocos2d::CCLayer *layer = dynamic_cast<cocos2d::CCLayer*>(p);
    CCAssert(layer, "CCLayerLoader::setProperty");
    
    if(name == "touchEnable")
    {
        if(value.IsBool())
        {
            layer->setTouchEnabled(value.GetBool());
        }
    }
    else if(name == "touchMode")
    {
        if(value.IsInt())
        {
            layer->setTouchMode((cocos2d::ccTouchesMode)value.GetInt());
        }
    }
    else if(name == "keypadEnable")
    {
        if(value.IsBool())
        {
            layer->setKeypadEnabled(value.GetBool());
        }
    }
    else
    {
        return base_class::setProperty(p, name, value, properties);
    }
    
    return true;
}
