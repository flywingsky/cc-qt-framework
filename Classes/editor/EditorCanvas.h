//
//  EditorCanvas.h
//  Editor
//
//  Created by C218-pc on 15/7/17.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Editor__EditorCanvas__
#define __Editor__EditorCanvas__

#include <uilib/UIWidget.h>
#include <rapidjson/document.h>
#include "EditorEventListener.h"

NS_CC_BEGIN
class CCDrawNode;
NS_CC_END

NS_UILIB_BEGIN
class FrameView;
NS_UILIB_END

namespace Editor
{
    class KeyEventListener;
    class KeyEvent;
    
    class CanvasPanel : public uilib::Widget
    {
    public:
        CREATE_FUNC(CanvasPanel)
        
        EventListener       listener;
        
        enum DragMode
        {
            DRAG_NONE = 0,
            DRAG_LEFT = 1 << 0,
            DRAG_RIGHT = 1 << 1,
            DRAG_TOP = 1 << 2,
            DRAG_BOTTOM = 1 << 3,
            DRAG_CENTER = 1 << 4,
        };
        
        CanvasPanel();
        ~CanvasPanel();

        void refreshTouchPriority();
        
        void setSelectedProperty(const std::string & name, const rapidjson::Value & value);
        
        cocos2d::CCNode* findNodeByPoint(cocos2d::CCNode* parent, const cocos2d::CCPoint & pt);
       
        bool onKeyEvent(const KeyEvent & event);
        
        void togglePreview();
        
        void setBackGroundColor(const cocos2d::ccColor4B & color);
        
    private:
        bool init() CC_OVERRIDE;
        void onMiddleWidgetClick(uilib::Widget* sender);
        void onMiddleWidgetTouch(uilib::Widget* sender, cocos2d::CCTouch *pTouch, int event);
        
        void doNodeSelect(const cocos2d::CCPoint & pt);
        void drawSelectedRect();
        void drawRect(const cocos2d::CCPoint & pt, const cocos2d::CCSize & size, const cocos2d::CCAffineTransform & world, const cocos2d::ccColor4F & color);
        
        void onNodeTouchMove(const cocos2d::CCPoint & pt, const cocos2d::CCPoint & old);
        void doNodeDrag(const cocos2d::CCPoint & delta);
        void doNodeResize(const cocos2d::CCPoint & delta);
        void doNodeScale(const cocos2d::CCPoint & delta);
        void doNodeRotate(const cocos2d::CCPoint & delta);
        
        bool handleDragEvent(const KeyEvent & event);
        bool handleRefreshEvent(const KeyEvent & event);
        
        bool onEventSetRoot(VariantVector & args);
        bool onEventSetSelect(VariantVector & args);
        bool onEventPropertyChange(VariantVector & args);
        
        uilib::Widget*    m_top;
        uilib::Widget*    m_middle;
        uilib::Widget*    m_bottom;
        uilib::FrameView* m_background;
        
        cocos2d::CCNode*    m_target;
        cocos2d::CCNode*    m_selectedNode;
        cocos2d::CCDrawNode*    m_drawdRect;
        int                 m_dragMode;
        
        KeyEventListener*   m_keyEventListener;
    };
}

#endif /* defined(__Editor__EditorCanvas__) */
