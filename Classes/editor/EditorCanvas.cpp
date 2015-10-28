//
//  EditorCanvas.cpp
//  Editor
//
//  Created by C218-pc on 15/7/17.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "EditorCanvas.h"
#include "Editor.h"
#include "uiloader/BaseLoader.h"

#include <2d/CCNode.h>
#include <2d/CCDrawNode.h>
#include <2d/CCScene.h>
#include <base/CCDirector.h>

#include <QMouseEvent>
#include <QKeyEvent>

namespace Editor
{
    namespace
    {
        const float DragDelta = 20.0f;
        const float KeyboardMoveDelta = 1.0f;

        cocos2d::Node* findNodeByPoint(cocos2d::Node* reference, const cocos2d::Point & pt)
        {
            if(!reference->isVisible())
            {
                return NULL;
            }

            auto & children = reference->getChildren();
            for(cocos2d::Node *child : children)
            {
                cocos2d::Node* selectedChild = findNodeByPoint(child, pt);
                if(NULL != selectedChild)
                {
                    return selectedChild;
                }
            }

            cocos2d::Point localPt = reference->convertToNodeSpace(pt);
            cocos2d::Rect rc(0, 0, reference->getContentSize().width, reference->getContentSize().height);
            if(rc.containsPoint(localPt))
            {
                return reference;
            }
            return NULL;
        }
    }

    
    Canvas::Canvas(QObject *parent)
        : QObject(parent)
        , m_dragMode(DRAG_NONE)
    {
        m_drawRect = cocos2d::DrawNode::create();
        Editor::instance()->getScene()->addChild(m_drawRect, 9999);
    }
    
    Canvas::~Canvas()
    {
    }

    void Canvas::onRootSet(cocos2d::Node *root)
    {
        m_rootNode = root;
    }

    void Canvas::onTargetSet(cocos2d::Node *target)
    {
        m_targetNode = target;
        drawSelectedRect();
    }

    void Canvas::onPopertyChange(PropertyParam &param)
    {
        setSelectedProperty(param.name, param.value);
    }

    void Canvas::onMouseEvent(QMouseEvent *event)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            cocos2d::Point pt(event->x(), event->y());
            m_lastMousePosition = cocos2d::Director::getInstance()->convertToUI(pt);
            doNodeSelect(m_lastMousePosition);
        }
        else if(event->type() == QEvent::MouseButtonRelease)
        {

        }
        else if(event->type() == QEvent::MouseMove)
        {
            if(event->buttons() & Qt::LeftButton)
            {
                cocos2d::Point pt(event->x(), event->y());
                pt = cocos2d::Director::getInstance()->convertToUI(pt);
                onNodeTouchMove(pt, m_lastMousePosition);
                m_lastMousePosition = pt;
            }
        }
    }

    void Canvas::onKeyEvent(QKeyEvent *event)
    {
        if(event->type() == QEvent::KeyRelease)
        {
            if(event->key() == Qt::Key_Control)
            {
                drawSelectedRect();
            }
            else if(event->key() == Qt::Key_Delete)
            {
                if(m_rootNode)
                {
                    emit signalDeleteNode(m_rootNode);
                }
            }
            else if(handleDragEvent(event))
            {

            }
        }
    }


    void Canvas::doNodeSelect(const cocos2d::Point & pt)
    {
        if(NULL == m_rootNode)
        {
            return;
        }
        cocos2d::Node * selected = NULL;
        
        bool canResize = false; // KeyEvent::getGlobalModifier() & KeyCode::MOD_CTRL;
        if(canResize)
        {
            selected = m_targetNode;
        }
        else
        {
            selected = findNodeByPoint(m_rootNode, pt);
            emit signalSetTarget(selected);
        }
        
        m_dragMode = DRAG_NONE;
        if(NULL == selected)
        {
            return;
        }

        cocos2d::Point localPt = selected->convertToNodeSpace(pt);
        cocos2d::Size size = selected->getContentSize();
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
    
    void Canvas::drawSelectedRect()
    {
        m_drawRect->clear();
        
        m_drawRect->setVisible(bool(m_targetNode));
        if(NULL == m_targetNode)
        {
            return;
        }
        
        const cocos2d::Size & size = m_targetNode->getContentSize();
        cocos2d::AffineTransform toWorld = m_targetNode->nodeToWorldTransform();
        
        if(false /*KeyEvent::getGlobalModifier() & KeyCode::MOD_CTRL*/)
        {
            cocos2d::Size sz(size);
            sz.width += DragDelta;
            sz.height += DragDelta;
            drawRect(cocos2d::Point::ZERO, sz, toWorld, cocos2d::Color4F(0.0f, 0.0f, 1.0f, 1.0f));
        }
        
        drawRect(cocos2d::Point::ZERO, size, toWorld, cocos2d::Color4F(1.0f, 0.0f, 0.0f, 1.0f));
    }
    
    void Canvas::drawRect(const cocos2d::Point & pt, const cocos2d::Size & size, const cocos2d::AffineTransform & world, const cocos2d::Color4F & color)
    {
        const int nPoints = 4;
        cocos2d::Point points[nPoints];
        points[0].setPoint(pt.x, pt.y);
        points[1].setPoint(pt.x, pt.y + size.height);
        points[2].setPoint(pt.x + size.width, pt.y + size.height);
        points[3].setPoint(pt.x + size.width, pt.y);
        
        cocos2d::AffineTransform toLocal = m_drawRect->worldToNodeTransform();
        cocos2d::AffineTransform transform = cocos2d::AffineTransformConcat(world, toLocal);
        for(int i = 0; i < nPoints; ++i)
        {
            points[i] = cocos2d::PointApplyAffineTransform(points[i], transform);
        }
        
        for(int i = 0; i < nPoints; ++i)
        {
            int start = i % nPoints;
            int end = (i + 1) % nPoints;
            
            m_drawRect->drawSegment(points[start], points[end], 1.0f, color);
        }
    }
    
    void Canvas::onNodeTouchMove(const cocos2d::Point & pt, const cocos2d::Point & old)
    {
        if(NULL == m_targetNode)
        {
            return;
        }
        
        cocos2d::AffineTransform t = m_targetNode->getParent()->worldToNodeTransform();
        cocos2d::Point localPt = cocos2d::PointApplyAffineTransform(pt, t);
        cocos2d::Point localOld = cocos2d::PointApplyAffineTransform(old, t);
        
        cocos2d::Point delta = localPt - localOld;
        
        if(m_dragMode == DRAG_CENTER)
        {
            doNodeDrag(delta);
        }
        else
        {
            doNodeResize(delta);
        }
    }

    void Canvas::doNodeDrag(const cocos2d::Point & delta)
    {
        rapidjson::Document::AllocatorType & allocator = Editor::instance()->getAllocator();
        
        cocos2d::Point position = m_targetNode->getPosition() + delta;
        position.x = roundf(position.x);
        position.y = roundf(position.y);
        
        rapidjson::Value value;
        value.SetArray();
        value.PushBack(position.x, allocator);
        value.PushBack(position.y, allocator);

        Editor::instance()->emitTargetPropertyChange("position", value);
    }

    void Canvas::doNodeResize(const cocos2d::Point & delta)
    {
        rapidjson::Document::AllocatorType & allocator = Editor::instance()->getAllocator();

        bool sizeChanged = false;
        cocos2d::Size newSize = m_targetNode->getContentSize();
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
        
        Editor::instance()->emitTargetPropertyChange("size", value);
    }

    void Canvas::doNodeScale(const cocos2d::Point & delta)
    {

    }

    void Canvas::doNodeRotate(const cocos2d::Point & delta)
    {

    }
    
    void Canvas::setSelectedProperty(const std::string & name, const rapidjson::Value & value)
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
    
    bool Canvas::handleDragEvent(QKeyEvent *event)
    {
        int modifier = event->modifiers();
        
        if(modifier == 0 || modifier == Qt::CTRL)
        {
            cocos2d::Point delta;
            m_dragMode = DRAG_NONE;
            switch (event->key())
            {
                case Qt::Key_Left:
                    delta.x -= KeyboardMoveDelta;
                    m_dragMode = DRAG_RIGHT;
                    break;
                    
                case Qt::Key_Right:
                    delta.x += KeyboardMoveDelta;
                    m_dragMode = DRAG_RIGHT;
                    break;
                    
                case Qt::Key_Up:
                    delta.y += KeyboardMoveDelta;
                    m_dragMode = DRAG_TOP;
                    break;
                    
                case Qt::Key_Down:
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

    void Canvas::togglePreview()
    {

    }
    
    void Canvas::setBackGroundColor(const cocos2d::Color4B & color)
    {

    }


} // end namespace Editor
