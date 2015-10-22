//
//  CCTextFieldExtLoader.h
//  Editor
//
//  Created by C218-pc on 15/8/11.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Editor__CCTextFieldExtLoader__
#define __Editor__CCTextFieldExtLoader__

#include "CCLabelLoader.h"

class CCTextFieldExtLoader : public CCLabelLoader
{
public:
    DECLARE_LOADER(CCTextFieldExtLoader);
    
    virtual cocos2d::Node * createObject(rapidjson::Value & config);
    virtual bool setProperty(cocos2d::Node *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties);
};

#endif /* defined(__Editor__CCTextFieldExtLoader__) */
