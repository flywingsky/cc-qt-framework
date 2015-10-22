//
//  UILayoutLoader.cpp
//  Clover
//
//  Created by C218-pc on 15/6/30.
//
//

#include "UILayoutLoader.h"
#include "UILayout.h"
#include "UIHelper.h"

namespace
{
    enum Deprecated_ItemAlign
    {
        ALIGN_FREE = 0,
        
        ALIGN_LEFT = 1, //b0
        ALIGN_HCENTER = 2, //b1
        ALIGN_RIGHT = 4, //b2
        
        ALIGN_TOP = 8, //b3
        ALIGN_VCENTER = 16, //b4
        ALIGN_BOTTOM = 32, //b5
        
        ALIGN_STRETCH = 64,
        
        ALIGN_CENTER = ALIGN_HCENTER | ALIGN_VCENTER,
        ALIGN_FORCE_DWORD = 0x7fffff
    };
    
    void oldAlign2new(int align, uilib::Layout::ItemHAlign & hAlign, uilib::Layout::ItemVAlign & vAlign)
    {
        using uilib::Layout;
        
        hAlign = Layout::H_ALIGN_FREE;
        vAlign = Layout::V_ALIGN_FREE;
        
        if(align & ALIGN_LEFT)
        {
            hAlign = Layout::H_ALIGN_LEFT;
        }
        else if(align & ALIGN_HCENTER)
        {
            hAlign = Layout::H_ALIGN_CENTER;
        }
        else if(align & ALIGN_RIGHT)
        {
            hAlign = Layout::H_ALIGN_RIGHT;
        }
        
        if(align & ALIGN_BOTTOM)
        {
            vAlign = Layout::V_ALIGN_BOTTOM;
        }
        else if(align & ALIGN_VCENTER)
        {
            vAlign = Layout::V_ALIGN_CENTER;
        }
        else if(align & ALIGN_TOP)
        {
            vAlign = Layout::V_ALIGN_TOP;
        }
        
        if(align & ALIGN_STRETCH)
        {
            vAlign = Layout::V_ALIGN_STRETCH;
            hAlign = Layout::H_ALIGN_STRETCH;
        }
    }
}

cocos2d::CCNode * UILayoutLoader::createObject(rapidjson::Value & config)
{
    return uilib::Layout::create();
}

bool UILayoutLoader::setProperty(cocos2d::CCNode *p, const std::string & name, const rapidjson::Value & value, rapidjson::Value & properties)
{
    uilib::Layout *layout = dynamic_cast<uilib::Layout*>(p);
    CCAssert(layout, "NodeLoader::setProperty");
    
    if(name == "layoutType")
    {
        if(value.IsInt())
        {
            layout->setLayoutType((uilib::Layout::LayoutType)value.GetInt());
        }
    }
    else if(name == "itemAlign")
    {
        int datas[2];
        if(helper::parseNumberArray(value, datas, 2))
        {
            layout->setItemHorizontalAlign(uilib::Layout::ItemHAlign(datas[0]));
            layout->setItemVerticalAlign(uilib::Layout::ItemVAlign(datas[1]));
        }
    }
    else if(name == "itemContentSize")
    {
        cocos2d::CCSize size;
        if(helper::parseValue(value, size))
        {
            layout->setItemContentSize(size);
        }
    }
    else if(name == "itemMargin")
    {
        if(value.IsNumber())
        {
            layout->setItemMargin(value.GetDouble());
        }
    }
    else if(name == "viewOffset")
    {
        cocos2d::CCPoint pt;
        if(helper::parseValue(value, pt))
        {
            layout->setViewOffset(pt);
        }
    }
    else if(name == "autoResize")
    {
        if(value.IsBool())
        {
            layout->setAutoResizeContent(value.GetBool());
        }
    }
    else
    {
        return base_class::setProperty(p, name, value, properties);
    }
    
    return true;
}

bool UILayoutLoader::upgradeProperty(rapidjson::Value & property, rapidjson::Value::AllocatorType & allocator)
{
    rapidjson::Value & value = property["itemAlign"];
    if(value.IsInt())
    {
        uilib::Layout::ItemHAlign hAlign;
        uilib::Layout::ItemVAlign vAlign;
        
        oldAlign2new(value.GetInt(), hAlign, vAlign);
        
        value.SetArray();
        value.PushBack(int(hAlign), allocator);
        value.PushBack(int(vAlign), allocator);
    }
    return base_class::upgradeProperty(property, allocator);
}
