//
//  CCLayerColorLoader.h
//  Editor
//
//  Created by C218-pc on 15/7/31.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Editor__CCLayerColorLoader__
#define __Editor__CCLayerColorLoader__

#include "CCLayerLoader.h"

class CCLayerColorLoader : public CCLayerLoader
{
public:
    DECLARE_LOADER(CCLayerColorLoader);
    
    virtual cocos2d::Node * createObject(rapidjson::Value & config);
    virtual bool setProperty(cocos2d::Node *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties);
};


#endif /* defined(__Editor__CCLayerColorLoader__) */
