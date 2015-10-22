//
//  LoaderAddin.cpp
//  Clover
//
//  Created by C218-pc on 15/6/29.
//
//

#include "CCNodeLoader.h"
#include "CCLayerLoader.h"
#include "CCSpriteLoader.h"
#include "CCScale9SpriteLoader.h"
#include "CCLabelLoader.h"
#include "CCTextFieldLoader.h"
#include "CCEditBoxLoader.h"
#include "CCLayerColorLoader.h"
#include "CCTextFieldExtLoader.h"

#include "UIWidgetLoader.h"
#include "UIButtonLoader.h"
#include "UILayoutLoader.h"
#include "UIScrollViewLoader.h"
#include "UIListViewLoader.h"
#include "UIImageViewLoader.h"
#include "UIFrameViewLoader.h"
#include "UILabelLoader.h"
#include "UITextFieldLoader.h"
#include "UIEditBoxLoader.h"
#include "UICheckBoxLoader.h"
#include "UITextFieldExtLoader.h"

void loadDefaultLoaders(UILoader *ld)
{
    ld->registerLoader("Node", new CCNodeLoader());
    ld->registerLoader("Layer", new CCLayerLoader());
    ld->registerLoader("Sprite", new CCSpriteLoader());
    ld->registerLoader("Label", new CCLabelLoader());
    ld->registerLoader("Scale9Sprite", new CCScale9SpriteLoader);
    ld->registerLoader("TextField", new CCTextFieldLoader());
    ld->registerLoader("EditBox", new CCEditBoxLoader());
    ld->registerLoader("LayerColor", new CCLayerColorLoader());
    
    ld->registerLoader("Widget", new UIWidgetLoader());
    ld->registerLoader("Button", new UIButtonLoader());
    ld->registerLoader("Layout", new UILayoutLoader());
    ld->registerLoader("ScrollView", new UIScrollViewLoader());
    ld->registerLoader("ListView", new UIListViewLoader());
    ld->registerLoader("ImageView", new UIImageViewLoader());
    ld->registerLoader("FrameView", new UIFrameViewLoader());
    ld->registerLoader("Label", new UILabelLoader());
    ld->registerLoader("TextField", new UITextFieldLoader());
    ld->registerLoader("EditBox", new UIEditBoxLoader());
    ld->registerLoader("CheckBox", new UICheckBoxLoader());
    ld->registerLoader("TextFieldExt", new UITextFieldExtLoader());
}
