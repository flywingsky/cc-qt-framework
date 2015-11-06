//
//  CCNodeLoader.h
//  Clover
//
//  Created by C218-pc on 15/6/26.
//
//

#ifndef __Clover__CCNodeLoader__
#define __Clover__CCNodeLoader__

#include "../BaseLoader.h"

class CCNodeLoader : public IBaseLoader
{
public:
    DECLARE_LOADER(CCNodeLoader)
    
    CCNodeLoader();
    virtual cocos2d::Node* createObject(rapidjson::Value & config) override;
};

#endif /* defined(__Clover__CCNodeLoader__) */
