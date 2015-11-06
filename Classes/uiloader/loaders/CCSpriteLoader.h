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
    DECLARE_LOADER(CCSpriteLoader)

    CCSpriteLoader();
    
    virtual cocos2d::Node* createObject(rapidjson::Value & config) override;
};

#endif /* defined(__Clover__CCSpriteLoader__) */
