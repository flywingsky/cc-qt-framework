//
//  CCLabelLoader.h
//  Clover
//
//  Created by C218-pc on 15/6/26.
//
//

#ifndef __Clover__CCLabelLoader__
#define __Clover__CCLabelLoader__

#include "CCSpriteLoader.h"

class CCLabelLoader : public CCSpriteLoader
{
public:
    DECLARE_LOADER(CCLabelLoader);
    
    virtual cocos2d::CCNode * createObject(rapidjson::Value & config);
    virtual bool setProperty(cocos2d::CCNode *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties);
};

#endif /* defined(__Clover__CCLabelLoader__) */
