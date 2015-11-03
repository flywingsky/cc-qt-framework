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
class QWheelEvent;

class GLWidget;

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
        
        Canvas(QObject *parent, GLWidget *view);
        ~Canvas();
        
        void togglePreview();
        void setBackGroundColor(const cocos2d::Color4B & color);
        
    signals:
        void signalSetTarget(cocos2d::Node *target);
        void signalDeleteNode(cocos2d::Node *node);

    public slots:
        virtual void onRootSet(cocos2d::Node *root);
        virtual void onTargetSet(cocos2d::Node *target);
        virtual void onPopertyChange(PropertyParam &param);
        virtual void onMouseEvent(QMouseEvent *event);
        virtual void onKeyEvent(QKeyEvent *event);
        virtual void onWheelEvent(QWheelEvent * event);
        virtual void onResize(float width, float height);

        virtual void onTick(float dt){}

    protected:

        virtual void drawSelectedRect();
        
        cocos2d::RefPtr<cocos2d::Node>      rootNode_;
        cocos2d::RefPtr<cocos2d::Node>      targetNode_;

        cocos2d::Point      lastMousePosition_;
    };
}

#endif /* defined(__Editor__EditorCanvas__) */
