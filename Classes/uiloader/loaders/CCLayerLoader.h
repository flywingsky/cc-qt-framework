//
//  CCLayerLoader.h
//  Clover
//
//  Created by C218-pc on 15/6/29.
//
//

#ifndef __Clover__LayerLoader__
#define __Clover__LayerLoader__

#include "CCNodeLoader.h"

class CCLayerLoader : public CCNodeLoader
{
public:
    DECLARE_LOADER(CCLayerLoader);
    
    virtual cocos2d::CCNode * createObject(rapidjson::Value & config);
    virtual bool setProperty(cocos2d::CCNode *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties);
};

#endif /* defined(__Clover__CCLayerLoader__) */
