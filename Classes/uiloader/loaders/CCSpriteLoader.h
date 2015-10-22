//
//  CCSpriteLoader.h
//  Clover
//
//  Created by C218-pc on 15/6/26.
//
//

#ifndef __Clover__CCSpriteLoader__
#define __Clover__CCSpriteLoader__

#include "CCNodeLoader.h"

class CCSpriteLoader : public CCNodeLoader
{
public:
    DECLARE_LOADER(CCSpriteLoader);
    
    virtual cocos2d::Node * createObject(rapidjson::Value & config);
    virtual bool setProperty(cocos2d::Node *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties);
};

#endif /* defined(__Clover__CCSpriteLoader__) */
