//
//  UIListViewLoader.h
//  Clover
//
//  Created by C218-pc on 15/7/6.
//
//

#ifndef __Clover__UIListViewLoader__
#define __Clover__UIListViewLoader__

#include "UIScrollViewLoader.h"

class UIListViewLoader : public UIScrollViewLoader
{
public:
    DECLARE_LOADER(UIListViewLoader);
    
    virtual cocos2d::CCNode * createObject(rapidjson::Value & config);
    virtual bool setProperty(cocos2d::CCNode *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties);
};

#endif /* defined(__Clover__UIListViewLoader__) */
