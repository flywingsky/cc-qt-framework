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
    DECLARE_LOADER(CCNodeLoader);
    
    virtual cocos2d::Node * createObject(rapidjson::Value & config);
    virtual bool setProperty(cocos2d::Node *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties);
};

#endif /* defined(__Clover__CCNodeLoader__) */
