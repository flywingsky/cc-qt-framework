//
//  UICheckBox.cpp
//  Editor
//
//  Created by C218-pc on 15/7/25.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "UICheckBoxLoader.h"

#include "uilib/UICheckBox.h"

cocos2d::CCNode * UICheckBoxLoader::createObject(rapidjson::Value & config)
{
    return uilib::CheckBox::create();
}

bool UICheckBoxLoader::setProperty(cocos2d::CCNode *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties)
{
    uilib::CheckBox * box = dynamic_cast<uilib::CheckBox*>(p);
    CCAssert(box != NULL, "UICheckBoxLoader::setProperty");
    
    if(name == "checked")
    {
        if(value.IsBool())
        {
            box->setChecked(value.GetBool());
        }
    }
    else if(name == "checkedImage")
    {
        std::string imageNormal, imagePressed, imageDisable;
        if(value.IsArray())
        {
            do
            {
                if(value.Size() < 1 || !value[0u].IsString()) break;
                imageNormal = value[0u].GetString();
                
                if(value.Size() < 2 || !value[1].IsString()) break;
                imagePressed = value[1].GetString();
                
                if(value.Size() < 3 || !value[2].IsString()) break;
                imageDisable = value[2].GetString();
                
            }while(0);
            box->setCheckedImages(imageNormal, imagePressed, imageDisable);
        }
    }
    else
    {
        return base_class::setProperty(p, name, value, properties);
    }
    
    return true;
}
