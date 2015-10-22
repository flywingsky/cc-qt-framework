//
//  CCCCTextFieldLoader.cpp
//  Clover
//
//  Created by C218-pc on 15/7/8.
//
//

#include "CCTextFieldLoader.h"
#include <text_input_node/CCTextFieldTTF.h>

#include "UIHelper.h"


cocos2d::CCNode * CCTextFieldLoader::createObject(rapidjson::Value & config)
{
    cocos2d::CCTextFieldTTF *p = new cocos2d::CCTextFieldTTF();
    if(!p->init())
    {
        delete p;
        return NULL;
    }
    p->autorelease();
    return p;
}

bool CCTextFieldLoader::setProperty(cocos2d::CCNode *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties)
{
    cocos2d::CCTextFieldTTF *text = dynamic_cast<cocos2d::CCTextFieldTTF*>(p);
    CCAssert(text, "CCTextFieldLoader::setProperty");
    
    if(name == "placeHolderText")
    {
        if(value.IsString())
        {
            text->setPlaceHolder(value.GetString());
        }
    }
    else if(name == "placeHolderColor")
    {
        cocos2d::ccColor3B cr;
        if(helper::parseValue(value, cr))
        {
            text->setColorSpaceHolder(cr);
        }
    }
    else if(name == "isSecureTextEntry")
    {
        if(value.IsBool())
        {
            text->setSecureTextEntry(value.GetBool());
        }
    }
    else
    {
        return base_class::setProperty(p, name, value, properties);
    }
    
    return true;
}
