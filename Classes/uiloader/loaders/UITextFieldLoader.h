//
//  TextFieldLoader.h
//  Clover
//
//  Created by C218-pc on 15/7/7.
//
//

#ifndef __Clover__TextFieldLoader__
#define __Clover__TextFieldLoader__

#include "UILabelLoader.h"

class UITextFieldLoader : public UILabelLoader
{
public:
    DECLARE_LOADER(UITextFieldLoader);
    
    virtual cocos2d::CCNode * createObject(rapidjson::Value & config);
    virtual bool setProperty(cocos2d::CCNode *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties);
};

#endif /* defined(__Clover__TextFieldLoader__) */
