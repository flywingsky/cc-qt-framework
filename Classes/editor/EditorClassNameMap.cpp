//
//  EditorClassNameMap.cpp
//  Editor
//
//  Created by C218-pc on 15/7/20.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "EditorClassNameMap.h"
#include "EditorPropertyMgr.h"

#include <GUI/CCEditBox/CCEditBox.h>

#include "CCTextFieldExt.h"

#include "UIWidget.h"
#include "UIButton.h"
#include "UILayout.h"
#include "UIScrollView.h"
#include "UIListView.h"
#include "UIImageView.h"
#include "UIFrameView.h"
#include "UILabel.h"
#include "UITextField.h"
#include "UIEditBox.h"
#include "UICheckBox.h"
#include "UILucidAnimation.h"
#include "UITextFieldExt.h"

#ifndef BUILD_FOR_EDITOR
#include "XSprite.h"
#include "XButton.h"
#include "XLabel.h"
#include "XScrollView.h"
#endif

namespace Editor
{
    void loadDefaultClassNameMap(PropertyMgr * pm)
    {
#define REGISTER_UI_NAME(CLASS, NAME) \
    pm->registerUIName(typeid(CLASS).name(), NAME)
  
        REGISTER_UI_NAME(cocos2d::CCNode, "CCNode");
        REGISTER_UI_NAME(cocos2d::CCLayer, "CCLayer");
        REGISTER_UI_NAME(cocos2d::CCSprite, "CCSprite");
        REGISTER_UI_NAME(cocos2d::CCLabelTTF, "CCLabel");
        REGISTER_UI_NAME(cocos2d::CCTextFieldTTF, "CCTextField");
        REGISTER_UI_NAME(cocos2d::extension::CCScale9Sprite, "CCScale9Sprite");
        REGISTER_UI_NAME(cocos2d::extension::CCEditBox, "CCEditBox");
        REGISTER_UI_NAME(cocos2d::CCLayerColor, "CCLayerColor");
        REGISTER_UI_NAME(cocos2d::CCTextFieldTTFExt, "CCTextFieldExt");
        
        REGISTER_UI_NAME(uilib::Widget, "Widget");
        REGISTER_UI_NAME(uilib::Button, "Button");
        REGISTER_UI_NAME(uilib::Layout, "Layout");
        REGISTER_UI_NAME(uilib::ScrollView, "ScrollView");
        REGISTER_UI_NAME(uilib::ListView, "ListView");
        REGISTER_UI_NAME(uilib::ImageView, "ImageView");
        REGISTER_UI_NAME(uilib::FrameView, "FrameView");
        REGISTER_UI_NAME(uilib::Label, "Label");
        REGISTER_UI_NAME(uilib::TextField, "TextField");
        REGISTER_UI_NAME(uilib::EditBox, "EditBox");
        REGISTER_UI_NAME(uilib::CheckBox, "CheckBox");
        REGISTER_UI_NAME(uilib::LucidAnimation, "LucidAnimation");
        REGISTER_UI_NAME(uilib::TextFieldExt, "TextFieldExt");
    
#ifndef BUILD_FOR_EDITOR
        REGISTER_UI_NAME(XLabel, "XLabel");
        REGISTER_UI_NAME(XSprite, "XSprite");
        REGISTER_UI_NAME(XButton, "XButton");
        REGISTER_UI_NAME(XScrollView, "XScrollView");
#endif

#undef REGISTER_UI_NAME
    }
}
