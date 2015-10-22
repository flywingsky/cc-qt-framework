//
//  TextFieldLoader.cpp
//  Clover
//
//  Created by C218-pc on 15/7/7.
//
//

#include "UITextFieldLoader.h"
#include "UITextField.h"
#include "UIHelper.h"

cocos2d::Node * UITextFieldLoader::createObject(rapidjson::Value & config)
{
    return uilib::TextField::create();
}

bool UITextFieldLoader::setProperty(cocos2d::Node *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties)
{
    uilib::TextField *text = dynamic_cast<uilib::TextField*>(p);
    CCAssert(text, "UITextFieldLoader::setProperty");
    
    if(name == "placeholderText")
    {
        if(value.IsString())
        {
            text->setPlaceholderText(value.GetString());
        }
    }
    else if(name == "placeholderColor")
    {
        cocos2d::Color3B cr;
        if(helper::parseValue(value, cr))
        {
            text->setPlaceholderColor(cr);
        }
    }
    else
    {
        return base_class::setProperty(p, name, value, properties);
    }
    
    return true;
}
