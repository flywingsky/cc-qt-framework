//
//  CCScale9SpriteLoader.h
//  Clover
//
//  Created by C218-pc on 15/7/6.
//
//

#ifndef __Clover__CCScale9SpriteLoader__
#define __Clover__CCScale9SpriteLoader__

#include "CCNodeLoader.h"

class CCScale9SpriteLoader : public CCNodeLoader
{
public:
    DECLARE_LOADER(CCScale9SpriteLoader);
    
    virtual cocos2d::Node * createObject(rapidjson::Value & config);
    virtual bool setProperty(cocos2d::Node *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties);
};

#endif /* defined(__Clover__CCScale9SpriteLoader__) */
