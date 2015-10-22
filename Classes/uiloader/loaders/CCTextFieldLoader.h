//
//  CCTextFieldLoader.h
//  Clover
//
//  Created by C218-pc on 15/7/8.
//
//

#ifndef __Clover__CCTextFieldLoader__
#define __Clover__CCTextFieldLoader__

#include "CCLabelLoader.h"

class CCTextFieldLoader : public CCLabelLoader
{
public:
    DECLARE_LOADER(CCTextFieldLoader);
    
    virtual cocos2d::Node * createObject(rapidjson::Value & config);
    virtual bool setProperty(cocos2d::Node *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties);
};

#endif /* defined(__Clover__CCTextFieldLoader__) */
