//
//  UIListViewLoader.cpp
//  Clover
//
//  Created by C218-pc on 15/7/6.
//
//

#include "UIListViewLoader.h"
#include "UIListView.h"

cocos2d::Node * UIListViewLoader::createObject(rapidjson::Value & config)
{
    return uilib::ListView::create();
}

bool UIListViewLoader::setProperty(cocos2d::Node *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties)
{
    uilib::ListView *list = dynamic_cast<uilib::ListView*>(p);
    CCAssert(list, "UIListViewLoader::setProperty");
    
    if(name == "childLayout")
    {
        if(value.IsString())
        {
            list->setChildLayoutFile(value.GetString());
        }
    }
    else if(name == "numItems")
    {
#ifdef BUILD_FOR_EDITOR
        list->removeAllChildren();
        if(value.IsInt() && value.GetInt() >= 0)
        {
            list->resizeList(value.GetInt());
        }
#endif
    }
    else
    {
        return base_class::setProperty(p, name, value, properties);
    }
    return true;
}
