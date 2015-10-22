//
//  EditorCanvas.cpp
//  Editor
//
//  Created by C218-pc on 15/7/17.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "EditorCanvas.h"
#include "EditorPanel.h"
#include "EditorKeyEvent.h"
#include "EditorKeyCode.h"

#include <draw_nodes/CCDrawNode.h>

#include <uilib/UIFrameView.h>

namespace Editor
{
    static const float DragDelta = 20.0f;
    static const float KeyboardMoveDelta = 1.0f;
    
    CanvasPanel::CanvasPanel()
        : m_top(NULL)
        , m_middle(NULL)
        , m_bottom(NULL)
        , m_target(NULL)
        , m_selectedNode(NULL)
        , m_dragMode(DRAG_NONE)
        , m_keyEventListener(NULL)
        , listener(this)
        , m_background(NULL)
    {
        
    }
    
    CanvasPanel::~CanvasPanel()
    {
        delete m_keyEventListener;
    }
    
    bool CanvasPanel::init()
    {
        if(!uilib::Widget::init())
            return false;
        
        if(!loadLayoutFromFile("editor/layout/canvas.json"))
            return false;
        
        if(!assignProperty(m_top, "top"))
            return false;

        if(!assignProperty(m_middle, "middle"))
            return false;
        
        if(!assignProperty(m_bottom, "bottom"))
            return false;
        
        if(!assignProperty(m_background, "background"))
            return false;
        
        m_middle->clickEventListener.set(this, (uilib::SEL_WidgetClick)&CanvasPanel::onMiddleWidgetClick);
        m_middle->touchEventListener.set(this, (uilib::SEL_WidgetTouchEvent)&CanvasPanel::onMiddleWidgetTouch);
        
        m_drawdRect = cocos2d::CCDrawNode::create();
        m_middle->addChild(m_drawdRect);
        m_drawdRect->setVisible(false);
        
        m_keyEventListener = new KeyEventListener(false);
        m_keyEventListener->keyEventListener.set(this, (SEL_KeyEvent)&CanvasPanel::onKeyEvent);
        addKeyEventListener(m_keyEventListener);
        
        listener.subscribeEvent(&g_editor->dispatcher, EE_SET_ROOT, (SEL_EditorEventMethod)&CanvasPanel::onEventSetRoot);
        listener.subscribeEvent(&g_editor->dispatcher, EE_SET_SELECT, (SEL_EditorEventMethod)&CanvasPanel::onEventSetSelect);
        listener.subscribeEvent(&g_editor->dispatcher, EE_PROPERTY_CHANGE, (SEL_EditorEventMethod)&CanvasPanel::onEventPropertyChange);
        return true;
    }
    
    void CanvasPanel::onMiddleWidgetClick(uilib::Widget* sender)
    {
        //LOG_DEBUG("middle widget clicked.");
    }
    
    void CanvasPanel::onMiddleWidgetTouch(uilib::Widget* sender, cocos2d::CCTouch *pTouch, int event)
    {
        CCAssert(sender == m_middle, "CanvasPanel::onMiddleWidgetTouch");
        
        switch(event)
        {
            case uilib::Widget::TOUCH_BEGAN:
                doNodeSelect(pTouch->getLocation());
                break;
            
            case uilib::Widget::TOUCH_MOVED:
                onNodeTouchMove(pTouch->getLocation(), sender->getLastTouchPoint());
                break;
                
            case uilib::Widget::TOUCH_ENDED:
            case uilib::Widget::TOUCH_CANCELED:
                break;
        }
    }
    
    cocos2d::CCNode* CanvasPanel::findNodeByPoint(cocos2d::CCNode* reference, const cocos2d::CCPoint & pt)
    {
        if(!reference->isVisible())
        {
            return NULL;
        }
        
        cocos2d::CCArray * children = reference->getChildren();
        if(children != NULL)
        {
            cocos2d::CCObject *obj;
            CCARRAY_FOREACH_REVERSE(children, obj)
            {
                cocos2d::CCNode* child = findNodeByPoint((cocos2d::CCNode*)obj, pt);
                if(NULL != child)
                {
                    return child;
                }
            }
        }
        
        cocos2d::CCPoint localPt = reference->convertToNodeSpace(pt);
        cocos2d::CCRect rc(0, 0, reference->getContentSize().width, reference->getContentSize().height);
        if(rc.containsPoint(localPt))
        {
            return reference;
        }
        return NULL;
    }

    void CanvasPanel::doNodeSelect(const cocos2d::CCPoint & pt)
    {
        if(NULL == m_target)
        {
            return;
        }
        cocos2d::CCNode * selected = NULL;
        
        bool canResize = KeyEvent::getGlobalModifier() & KeyCode::MOD_CTRL;
        if(canResize)
        {
            selected = m_selectedNode;
        }
        else
        {
            selected = findNodeByPoint(m_target, pt);
            g_editor->setSelectedNode(selected);
        }
        
        m_dragMode = DRAG_NONE;
        if(NULL == selected)
        {
            return;
        }

        cocos2d::CCPoint localPt = selected->convertToNodeSpace(pt);
        cocos2d::CCSize size = selected->getContentSize();
        if(canResize)
        {
            size.width += DragDelta;
            size.height += DragDelta;
        }
        
        if(localPt.x < 0 || localPt.x > size.width ||
           localPt.y < 0 || localPt.y > size.height)
        {
            return;
        }
        
        if(canResize)
        {
#if 0 // resize left and bottom is disabled.
            if(localPt.x <= DragDelta)
            {
                m_dragMode |= DRAG_LEFT;
            }
            if(localPt.y <= DragDelta)
            {
                m_dragMode |= DRAG_BOTTOM;
            }
#endif
            if(localPt.x >= size.width - DragDelta)
            {
                m_dragMode |= DRAG_RIGHT;
            }
            if(localPt.y >= size.height - DragDelta)
            {
                m_dragMode |= DRAG_TOP;
            }
        }
        
        if(m_dragMode & DRAG_LEFT && m_dragMode & DRAG_RIGHT)
        {
            m_dragMode = DRAG_CENTER;
        }
        if(m_dragMode & DRAG_TOP && m_dragMode & DRAG_BOTTOM)
        {
           m_dragMode = DRAG_CENTER;
        }
        if(m_dragMode == DRAG_NONE)
        {
            m_dragMode = DRAG_CENTER;
        }
    }
    
    void CanvasPanel::drawSelectedRect()
    {
        m_drawdRect->clear();
        
        m_drawdRect->setVisible(m_selectedNode != NULL);
        if(NULL == m_selectedNode)
        {
            return;
        }
        
        const cocos2d::CCSize & size = m_selectedNode->getContentSize();
        cocos2d::CCAffineTransform toWorld = m_selectedNode->nodeToWorldTransform();
        
        if(KeyEvent::getGlobalModifier() & KeyCode::MOD_CTRL)
        {
            cocos2d::CCSize sz(size);
            sz.width += DragDelta;
            sz.height += DragDelta;
            drawRect(cocos2d::CCPointZero, sz, toWorld, cocos2d::ccc4f(0.0f, 0.0f, 1.0f, 1.0f));
        }
        
        drawRect(cocos2d::CCPointZero, size, toWorld, cocos2d::ccc4f(1.0f, 0.0f, 0.0f, 1.0f));
    }
    
    void CanvasPanel::drawRect(const cocos2d::CCPoint & pt, const cocos2d::CCSize & size, const cocos2d::CCAffineTransform & world, const cocos2d::ccColor4F & color)
    {
        const int nPoints = 4;
        cocos2d::CCPoint points[nPoints];
        points[0].setPoint(pt.x, pt.y);
        points[1].setPoint(pt.x, pt.y + size.height);
        points[2].setPoint(pt.x + size.width, pt.y + size.height);
        points[3].setPoint(pt.x + size.width, pt.y);
        
        cocos2d::CCAffineTransform toLocal = m_drawdRect->worldToNodeTransform();
        cocos2d::CCAffineTransform transform = cocos2d::CCAffineTransformConcat(world, toLocal);
        for(int i = 0; i < nPoints; ++i)
        {
            points[i] = CCPointApplyAffineTransform(points[i], transform);
        }
        
        for(int i = 0; i < nPoints; ++i)
        {
            int start = i % nPoints;
            int end = (i + 1) % nPoints;
            
            m_drawdRect->drawSegment(points[start], points[end], 1.0f, color);
        }
    }
    
    void CanvasPanel::onNodeTouchMove(const cocos2d::CCPoint & pt, const cocos2d::CCPoint & old)
    {
        if(KeyEvent::getGlobalModifier() & KeyCode::MOD_CMD)
        {
            setPosition(getPosition() + (pt - old));
            return;
        }
        
        if(NULL == m_selectedNode)
        {
            return;
        }
        
        cocos2d::CCAffineTransform t = m_selectedNode->getParent()->worldToNodeTransform();
        cocos2d::CCPoint localPt = cocos2d::CCPointApplyAffineTransform(pt, t);
        cocos2d::CCPoint localOld = cocos2d::CCPointApplyAffineTransform(old, t);
        
        cocos2d::CCPoint delta = localPt - localOld;
        
        if(m_dragMode == DRAG_CENTER)
        {
            doNodeDrag(delta);
        }
        else
        {
            doNodeResize(delta);
        }
    }

    void CanvasPanel::doNodeDrag(const cocos2d::CCPoint & delta)
    {
        rapidjson::Document::AllocatorType & allocator = g_editor->getAllocator();
        
        cocos2d::CCPoint position = m_selectedNode->getPosition() + delta;
        position.x = roundf(position.x);
        position.y = roundf(position.y);
        
        rapidjson::Value value;
        value.SetArray();
        value.PushBack(position.x, allocator);
        value.PushBack(position.y, allocator);
        
        g_editor->notifyPropertyChange(this, "position", value);
        drawSelectedRect();
    }

    void CanvasPanel::doNodeResize(const cocos2d::CCPoint & delta)
    {
        rapidjson::Document::AllocatorType & allocator = g_editor->getAllocator();

        bool sizeChanged = false;
        cocos2d::CCSize newSize = m_selectedNode->getContentSize();
        if(m_dragMode & DRAG_RIGHT)
        {
            newSize.width += delta.x;
            sizeChanged = true;
        }
        if(m_dragMode & DRAG_TOP)
        {
            newSize.height += delta.y;
            sizeChanged = true;
        }
        
        if(!sizeChanged)
        {
            return;
        }
        
        newSize.width = std::max(DragDelta, roundf(newSize.width));
        newSize.height = std::max(DragDelta, roundf(newSize.height));
        
        rapidjson::Value value;
        value.SetArray();
        value.PushBack(newSize.width, allocator);
        value.PushBack(newSize.height, allocator);
        
        g_editor->notifyPropertyChange(this, "size", value);
        drawSelectedRect();
    }

    void CanvasPanel::doNodeScale(const cocos2d::CCPoint & delta)
    {

    }

    void CanvasPanel::doNodeRotate(const cocos2d::CCPoint & delta)
    {

    }
    
    void CanvasPanel::setSelectedProperty(const std::string & name, const rapidjson::Value & value)
    {
        if(name == "position")
        {
            
        }
        else if(name == "size")
        {
            
        }
        else if(name == "scale")
        {
            
        }
        else if(name == "rotation")
        {
            
        }
        else if(name == "percentPosition")
        {
            
        }
        else if(name == "percentSize")
        {
            
        }
        else
        {
            return;
        }
        
        drawSelectedRect();
    }
    
    void CanvasPanel::refreshTouchPriority()
    {
        //cocos2d-x bug: the last added touch listener will handle event first.
        m_middle->refreshTouchPriority();
        m_top->refreshTouchPriority();
    }
    
    bool CanvasPanel::onKeyEvent(const KeyEvent & event)
    {
        if(event.isKeyDown())
        {
            if(handleRefreshEvent(event))
                return true;
            
            if(handleDragEvent(event))
                return true;
        }
        else if(event.isModifier())
        {
            if(event.key == KeyCode::KEY_Control)
            {
                drawSelectedRect();
            }
        }
        return false;
    }
    
    bool CanvasPanel::handleDragEvent(const KeyEvent & event)
    {
        int modifier = event.modifier & (~KeyCode::MOD_NUMERIC);
        
        if(modifier == 0 || modifier == KeyCode::MOD_CTRL)
        {
            cocos2d::CCPoint delta;
            m_dragMode = DRAG_NONE;
            switch (event.key)
            {
                case KeyCode::KEY_LeftArrow:
                    delta.x -= KeyboardMoveDelta;
                    m_dragMode = DRAG_RIGHT;
                    break;
                    
                case KeyCode::KEY_RightArrow:
                    delta.x += KeyboardMoveDelta;
                    m_dragMode = DRAG_RIGHT;
                    break;
                    
                case KeyCode::KEY_UpArrow:
                    delta.y += KeyboardMoveDelta;
                    m_dragMode = DRAG_TOP;
                    break;
                    
                case KeyCode::KEY_DownArrow:
                    delta.y -= KeyboardMoveDelta;
                    m_dragMode = DRAG_TOP;
                    break;
                    
                default:
                    break;
            }
            
            if(m_dragMode != DRAG_NONE)
            {
                if(modifier == 0)
                {
                    doNodeDrag(delta);
                }
                else
                {
                    doNodeResize(delta);
                }
                return true;
            }
        }
        return false;
    }
    
    bool CanvasPanel::handleRefreshEvent(const KeyEvent & event)
    {
        if(event.modifier == 0 && event.key == KeyCode::KEY_F5)
        {
            if(m_selectedNode)
            {
                uilib::Widget * widget = dynamic_cast<uilib::Widget*>(m_selectedNode);
                if(widget != NULL)
                {
                    widget->requireLayout();
                }
            }
            return true;
        }
        return false;
    }

    void CanvasPanel::togglePreview()
    {
        m_middle->setVisible(!m_middle->isVisible());
    }
    
    void CanvasPanel::setBackGroundColor(const cocos2d::ccColor4B & color)
    {
        m_background->setColor(cocos2d::ccc3(color.r, color.g, color.b));
        m_background->setOpacity(color.a);
    }
    
    bool CanvasPanel::onEventSetRoot(VariantVector & args)
    {
        cocos2d::CCNode * target = args[0].as_ptr<cocos2d::CCNode>();
        
        if(m_target != NULL)
        {
            m_bottom->removeChild(m_target);
        }
        
        m_target = target;
        if(m_target != NULL)
        {
            m_bottom->addChild(m_target);
        }
        
        refreshTouchPriority();
        return false;
    }
    
    bool CanvasPanel::onEventSetSelect(VariantVector & args)
    {
        cocos2d::CCNode *selected = args[0].as_ptr<cocos2d::CCNode>();
        
        m_selectedNode = selected;
        drawSelectedRect();
        return false;
    }
    
    bool CanvasPanel::onEventPropertyChange(VariantVector & args)
    {
        const cocos2d::CCNode* sender = args[0].as_ptr<cocos2d::CCNode>();
        if(sender != this)
        {
            const std::string * name = args[1].as_ptr<std::string>();
            const rapidjson::Value *value = args[2].as_ptr<rapidjson::Value>();
            
            setSelectedProperty(*name, *value);
        }
        return false;
    }


} // end namespace Editor
