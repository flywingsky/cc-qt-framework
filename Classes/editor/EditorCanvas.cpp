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
#include "glwidget.h"

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
            for(auto it = children.rbegin(); it != children.rend(); ++it)
            {
                cocos2d::Node* selectedChild = findNodeByPoint(*it, pt);
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

    
    Canvas::Canvas(QObject *parent, GLWidget *view)
        : QObject(parent)
        , dragMode_(DRAG_NONE)
    {
        drawRect_ = cocos2d::DrawNode::create();
        Editor::instance()->getScene()->addChild(drawRect_, 9999);

        connect(view, SIGNAL(signalMouseEvent(QMouseEvent*)), this, SLOT(onMouseEvent(QMouseEvent*)));
        connect(view, SIGNAL(signalKeyEvent(QKeyEvent*)), this, SLOT(onKeyEvent(QKeyEvent*)));
    }
    
    Canvas::~Canvas()
    {
    }

    void Canvas::onRootSet(cocos2d::Node *root)
    {
        rootNode_ = root;
    }

    void Canvas::onTargetSet(cocos2d::Node *target)
    {
        targetNode_ = target;
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
            lastMousePosition_ = cocos2d::Director::getInstance()->convertToUI(pt);
            doNodeSelect(lastMousePosition_);
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
                onNodeTouchMove(pt, lastMousePosition_);
                lastMousePosition_ = pt;
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
                if(rootNode_)
                {
                    emit signalDeleteNode(rootNode_);
                }
            }
            else if(handleDragEvent(event))
            {

            }
        }
    }


    void Canvas::doNodeSelect(const cocos2d::Point & pt)
    {
        if(NULL == rootNode_)
        {
            return;
        }
        cocos2d::Node * selected = NULL;
        
        bool canResize = false; // KeyEvent::getGlobalModifier() & KeyCode::MOD_CTRL;
        if(canResize)
        {
            selected = targetNode_;
        }
        else
        {
            selected = findNodeByPoint(rootNode_, pt);
            if(selected != targetNode_)
            {
                emit signalSetTarget(selected);
            }
        }
        
        dragMode_ = DRAG_NONE;
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
                dragMode_ |= DRAG_LEFT;
            }
            if(localPt.y <= DragDelta)
            {
                dragMode_ |= DRAG_BOTTOM;
            }
#endif
            if(localPt.x >= size.width - DragDelta)
            {
                dragMode_ |= DRAG_RIGHT;
            }
            if(localPt.y >= size.height - DragDelta)
            {
                dragMode_ |= DRAG_TOP;
            }
        }
        
        if(dragMode_ & DRAG_LEFT && dragMode_ & DRAG_RIGHT)
        {
            dragMode_ = DRAG_CENTER;
        }
        if(dragMode_ & DRAG_TOP && dragMode_ & DRAG_BOTTOM)
        {
           dragMode_ = DRAG_CENTER;
        }
        if(dragMode_ == DRAG_NONE)
        {
            dragMode_ = DRAG_CENTER;
        }
    }
    
    void Canvas::drawSelectedRect()
    {
        drawRect_->clear();
        
        drawRect_->setVisible(bool(targetNode_));
        if(NULL == targetNode_)
        {
            return;
        }
        
        const cocos2d::Size & size = targetNode_->getContentSize();
        cocos2d::AffineTransform toWorld = targetNode_->nodeToWorldTransform();
        
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
        
        cocos2d::AffineTransform toLocal = drawRect_->worldToNodeTransform();
        cocos2d::AffineTransform transform = cocos2d::AffineTransformConcat(world, toLocal);
        for(int i = 0; i < nPoints; ++i)
        {
            points[i] = cocos2d::PointApplyAffineTransform(points[i], transform);
        }
        
        for(int i = 0; i < nPoints; ++i)
        {
            int start = i % nPoints;
            int end = (i + 1) % nPoints;
            
            drawRect_->drawSegment(points[start], points[end], 1.0f, color);
        }
    }
    
    void Canvas::onNodeTouchMove(const cocos2d::Point & pt, const cocos2d::Point & old)
    {
        if(NULL == targetNode_)
        {
            return;
        }
        
        cocos2d::AffineTransform t = targetNode_->getParent()->worldToNodeTransform();
        cocos2d::Point localPt = cocos2d::PointApplyAffineTransform(pt, t);
        cocos2d::Point localOld = cocos2d::PointApplyAffineTransform(old, t);
        
        cocos2d::Point delta = localPt - localOld;
        
        if(dragMode_ == DRAG_CENTER)
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
        
        cocos2d::Point position = targetNode_->getPosition() + delta;
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
        cocos2d::Size newSize = targetNode_->getContentSize();
        if(dragMode_ & DRAG_RIGHT)
        {
            newSize.width += delta.x;
            sizeChanged = true;
        }
        if(dragMode_ & DRAG_TOP)
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
            dragMode_ = DRAG_NONE;
            switch (event->key())
            {
                case Qt::Key_Left:
                    delta.x -= KeyboardMoveDelta;
                    dragMode_ = DRAG_RIGHT;
                    break;
                    
                case Qt::Key_Right:
                    delta.x += KeyboardMoveDelta;
                    dragMode_ = DRAG_RIGHT;
                    break;
                    
                case Qt::Key_Up:
                    delta.y += KeyboardMoveDelta;
                    dragMode_ = DRAG_TOP;
                    break;
                    
                case Qt::Key_Down:
                    delta.y -= KeyboardMoveDelta;
                    dragMode_ = DRAG_TOP;
                    break;
                    
                default:
                    break;
            }
            
            if(dragMode_ != DRAG_NONE)
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
