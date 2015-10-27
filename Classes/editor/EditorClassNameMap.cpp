//
//  EditorClassNameMap.cpp
//  Editor
//
//  Created by C218-pc on 15/7/20.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "EditorClassNameMap.h"
#include "EditorPropertyTree.h"

#include <cocos2d.h>

#include <ui/CocosGUI.h>

namespace Editor
{
    void loadDefaultClassNameMap(PropertyTreeMgr * pm)
    {
#define REGISTER_UI_NAME(CLASS, NAME) \
    pm->registerUIName(typeid(CLASS).name(), NAME)
  
        REGISTER_UI_NAME(cocos2d::Node, "Node");
        REGISTER_UI_NAME(cocos2d::Scene, "Scene");
        REGISTER_UI_NAME(cocos2d::Layer, "Layer");
        REGISTER_UI_NAME(cocos2d::Sprite, "Sprite");
        REGISTER_UI_NAME(cocos2d::LabelTTF, "Label");
        REGISTER_UI_NAME(cocos2d::TextFieldTTF, "TextField");
        REGISTER_UI_NAME(cocos2d::LayerColor, "LayerColor");
        
        REGISTER_UI_NAME(cocos2d::ui::Widget, "Widget");
        REGISTER_UI_NAME(cocos2d::ui::Button, "Button");
        REGISTER_UI_NAME(cocos2d::ui::Layout, "Layout");
        REGISTER_UI_NAME(cocos2d::ui::ScrollView, "ScrollView");
        REGISTER_UI_NAME(cocos2d::ui::ListView, "ListView");
        REGISTER_UI_NAME(cocos2d::ui::ImageView, "ImageView");
//        REGISTER_UI_NAME(cocos2d::ui::FrameView, "FrameView");
//        REGISTER_UI_NAME(cocos2d::ui::Label, "Label");
        REGISTER_UI_NAME(cocos2d::ui::TextField, "TextField");
        REGISTER_UI_NAME(cocos2d::ui::EditBox, "EditBox");
        REGISTER_UI_NAME(cocos2d::ui::CheckBox, "CheckBox");
        REGISTER_UI_NAME(cocos2d::ui::Scale9Sprite, "Scale9Sprite");
        REGISTER_UI_NAME(cocos2d::ui::EditBox, "EditBox");

#undef REGISTER_UI_NAME
    }
}
