//
//  UILabelLoader.h
//  Clover
//
//  Created by C218-pc on 15/7/7.
//
//

#ifndef __Clover__UILabelLoader__
#define __Clover__UILabelLoader__

#include "UIWidgetLoader.h"

class UILabelLoader : public UIWidgetLoader
{
public:
    DECLARE_LOADER(UILabelLoader);
    
    virtual cocos2d::CCNode * createObject(rapidjson::Value & config);
    virtual bool setProperty(cocos2d::CCNode *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties);
};

#endif /* defined(__Clover__UILabelLoader__) */
