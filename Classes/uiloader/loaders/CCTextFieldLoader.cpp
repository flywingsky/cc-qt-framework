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


cocos2d::Node * CCTextFieldLoader::createObject(rapidjson::Value & config)
{
    cocos2d::TextFieldTTF *p = new cocos2d::TextFieldTTF();
    if(!p->init())
    {
        delete p;
        return NULL;
    }
    p->autorelease();
    return p;
}

bool CCTextFieldLoader::setProperty(cocos2d::Node *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties)
{
    cocos2d::TextFieldTTF *text = dynamic_cast<cocos2d::TextFieldTTF*>(p);
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
        cocos2d::Color3B cr;
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
