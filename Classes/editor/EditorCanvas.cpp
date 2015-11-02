//
//  EditorCanvas.cpp
//  Editor
//
//  Created by C218-pc on 15/7/17.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "EditorCanvas.h"
#include "glwidget.h"

#include <2d/CCNode.h>

namespace Editor
{

    Canvas::Canvas(QObject *parent, GLWidget *view)
        : QObject(parent)
    {
        connect(view, SIGNAL(signalMouseEvent(QMouseEvent*)), this, SLOT(onMouseEvent(QMouseEvent*)));
        connect(view, SIGNAL(signalKeyEvent(QKeyEvent*)), this, SLOT(onKeyEvent(QKeyEvent*)));
        connect(view, SIGNAL(signalResize(float,float)), this, SLOT(onResize(float,float)));
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

    }

    void Canvas::onMouseEvent(QMouseEvent *event)
    {

    }

    void Canvas::onKeyEvent(QKeyEvent *event)
    {

    }

    void Canvas::onResize(float width, float height)
    {

    }

    void Canvas::drawSelectedRect()
    {

    }

    void Canvas::togglePreview()
    {

    }
    
    void Canvas::setBackGroundColor(const cocos2d::Color4B & color)
    {

    }


} // end namespace Editor
