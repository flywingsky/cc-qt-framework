//
//  UIWidgetLoader.cpp
//  Clover
//
//  Created by C218-pc on 15/7/2.
//
//

#include "UIWidgetLoader.h"
#include "UIWidget.h"
#include "UIHelper.h"

cocos2d::CCNode * UIWidgetLoader::createObject(rapidjson::Value & config)
{
    return uilib::Widget::create();
}

bool UIWidgetLoader::setProperty(cocos2d::CCNode *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties)
{
    uilib::Widget *widget = dynamic_cast<uilib::Widget*>(p);
    CCAssert(widget, "UIWidgetLoader::setProperty");
    
    if(name == "touchEnable")
    {
        if(value.IsBool())
        {
            widget->setTouchEnabled(value.GetBool());
        }
    }
    else if(name == "touchPriority")
    {
        if(value.IsInt())
        {
            widget->setTouchPriority(value.GetInt());
        }
    }
    else if(name == "clipEnable")
    {
        if(value.IsBool())
        {
            widget->setClipEnable(value.GetBool());
        }
    }
    else if(name == "clipOffset")
    {
        float offsets[4];
        if(helper::parseNumberArray(value, offsets, 4))
        {
            widget->setClipOffset(offsets);
        }
    }
    else if(name == "propagateTouchEvents")
    {
        if(value.IsBool())
        {
            widget->setPropagateTouchEvents(value.GetBool());
        }
    }
    else if(name == "percentPosition")
    {
        cocos2d::CCPoint pt;
        if(helper::parseValue(value, pt))
        {
            widget->setPercentPosition(pt);
        }
    }
    else if(name == "percentSize")
    {
        cocos2d::CCSize size;
        if(helper::parseValue(value, size))
        {
            widget->setPercentSize(size);
        }
    }
    else if(name == "flexedSize")
    {
        if(value.IsBool())
        {
            widget->setFlexedSizeEnable(value.GetBool());
        }
    }
    else if(name == "minSize")
    {
        cocos2d::CCSize size;
        if(helper::parseValue(value, size))
        {
            widget->setMinSize(size);
        }
    }
    else if(name == "maxSize")
    {
        cocos2d::CCSize size;
        if(helper::parseValue(value, size))
        {
            widget->setMaxSize(size);
        }
    }
    else if(name == "sizeType")
    {
        int types[2];
        if(helper::parseNumberArray(value, types, 2))
        {
            widget->setHSizeType((uilib::Widget::SizeType)types[0]);
            widget->setVSizeType((uilib::Widget::SizeType)types[1]);
        }
    }
    else if(name == "posType")
    {
        int types[2];
        if(helper::parseNumberArray(value, types, 2))
        {
            widget->setHPositionType((uilib::Widget::PositionType)types[0]);
            widget->setVPositionType((uilib::Widget::PositionType)types[1]);
        }
    }
    else
    {
        return base_class::setProperty(p, name, value, properties);
    }
    
    return true;
}

bool UIWidgetLoader::upgradeProperty(rapidjson::Value & property, rapidjson::Value::AllocatorType & allocator)
{
    rapidjson::Value *jvalue = NULL;
    
    jvalue = &property["isPercentPosition"];
    if(jvalue->IsBool())
    {
        uilib::Widget::PositionType tp = jvalue->GetBool() ? uilib::Widget::POS_RELATIVE : uilib::Widget::POS_ABSOLUTE;
        property.RemoveMemberStable("isPercentPosition");
        
        if(!property.HasMember("posType"))
        {
            rapidjson::Value newValue(rapidjson::kArrayType);
            newValue.PushBack(int(tp), allocator);
            newValue.PushBack(int(tp), allocator);
            
            property.AddMember("posType", newValue, allocator);
        }
    }
    
    jvalue = &property["isPercentSize"];
    if(jvalue->IsBool())
    {
        uilib::Widget::SizeType tp = jvalue->GetBool() ? uilib::Widget::SIZE_RELATIVE : uilib::Widget::SIZE_ABSOLUTE;
        property.RemoveMemberStable("isPercentSize");
        
        if(!property.HasMember("sizeType"))
        {
            rapidjson::Value newValue(rapidjson::kArrayType);
            newValue.PushBack(int(tp), allocator);
            newValue.PushBack(int(tp), allocator);
            
            property.AddMember("sizeType", newValue, allocator);
        }
    }

    return base_class::upgradeProperty(property, allocator);
}

void UIWidgetLoader::trimProperty(rapidjson::Value & property, rapidjson::Value::AllocatorType & allocator)
{
    rapidjson::Value *jvalue = NULL;
    
    jvalue = &property["posType"];
    if(jvalue->IsArray())
    {
        int datas[2];
        if(helper::parseNumberArray(*jvalue, datas, 2))
        {
            if(datas[0] == uilib::Widget::POS_ABSOLUTE &&
               datas[1] == uilib::Widget::POS_ABSOLUTE)
            {
                property.RemoveMemberStable("percentPosition");
            }
            else if(datas[0] != uilib::Widget::POS_ABSOLUTE &&
                    datas[1] != uilib::Widget::POS_ABSOLUTE)
            {
                property.RemoveMemberStable("position");
            }
        }
    }
    else
    {
        property.RemoveMemberStable("percentPosition");
    }
    
    jvalue = &property["sizeType"];
    if(jvalue->IsArray())
    {
        int datas[2];
        if(helper::parseNumberArray(*jvalue, datas, 2))
        {
            if(datas[0] == uilib::Widget::SIZE_ABSOLUTE &&
               datas[1] == uilib::Widget::SIZE_ABSOLUTE)
            {
                property.RemoveMemberStable("percentSize");
            }
            else if(datas[0] != uilib::Widget::SIZE_ABSOLUTE &&
                    datas[1] != uilib::Widget::SIZE_ABSOLUTE)
            {
                property.RemoveMemberStable("size");
            }
        }
    }
    else
    {
        property.RemoveMemberStable("percentSize");
    }
    
    base_class::trimProperty(property, allocator);
}
