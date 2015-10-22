//
//  UIScrollViewLoader.h
//  Clover
//
//  Created by C218-pc on 15/7/2.
//
//

#ifndef __Clover__UIScrollViewLoader__
#define __Clover__UIScrollViewLoader__

#include "UIWidgetLoader.h"

class UIScrollViewLoader : public UIWidgetLoader
{
public:
    DECLARE_LOADER(UIScrollViewLoader);
    
    virtual cocos2d::Node * createObject(rapidjson::Value & config);
    virtual bool setProperty(cocos2d::Node *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties);
};

#endif /* defined(__Clover__UIScrollViewLoader__) */
