//
//  EditorCanvas.h
//  Editor
//
//  Created by C218-pc on 15/7/17.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Editor__EditorCanvas__
#define __Editor__EditorCanvas__

#include <QObject>
#include <rapidjson/document.h>

#include <platform/CCPlatformMacros.h>
#include <base/ccTypes.h>
#include <base/CCRefPtr.h>

NS_CC_BEGIN
class Node;
class DrawNode;
class AffineTransform;
NS_CC_END

class PropertyParam;

class QMouseEvent;
class QKeyEvent;

namespace Editor
{

    class Canvas : public QObject
    {
        Q_OBJECT
    public:
        
        enum DragMode
        {
            DRAG_NONE   = 0,
            DRAG_LEFT   = 1 << 0,
            DRAG_RIGHT  = 1 << 1,
            DRAG_TOP    = 1 << 2,
            DRAG_BOTTOM = 1 << 3,
            DRAG_CENTER = 1 << 4,
        };
        
        Canvas(QObject *parent);
        ~Canvas();
        
        void setSelectedProperty(const std::string & name, const rapidjson::Value & value);

        void togglePreview();
        void setBackGroundColor(const cocos2d::Color4B & color);
        
    signals:
        void signalSetTarget(cocos2d::Node *target);
        void signalDeleteNode(cocos2d::Node *node);

    public slots:
        void onRootSet(cocos2d::Node *root);
        void onTargetSet(cocos2d::Node *target);
        void onPopertyChange(PropertyParam &param);
        void onMouseEvent(QMouseEvent *event);
        void onKeyEvent(QKeyEvent *event);

    private:
        
        void doNodeSelect(const cocos2d::Point & pt);
        void drawSelectedRect();
        void drawRect(const cocos2d::Point & pt, const cocos2d::Size & size, const cocos2d::AffineTransform & world, const cocos2d::Color4F & color);
        
        void onNodeTouchMove(const cocos2d::Point & pt, const cocos2d::Point & old);
        void doNodeDrag(const cocos2d::Point & delta);
        void doNodeResize(const cocos2d::Point & delta);
        void doNodeScale(const cocos2d::Point & delta);
        void doNodeRotate(const cocos2d::Point & delta);
        
        bool handleDragEvent(QKeyEvent *event);
        

        cocos2d::RefPtr<cocos2d::Node>      rootNode_;
        cocos2d::RefPtr<cocos2d::Node>      targetNode_;
        cocos2d::RefPtr<cocos2d::DrawNode>  drawRect_;

        int                 dragMode_;
        cocos2d::Point      lastMousePosition_;
    };
}

#endif /* defined(__Editor__EditorCanvas__) */
