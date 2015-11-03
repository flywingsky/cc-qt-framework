#include "EditorCanvas3D.h"

#include "DrawNode3D.h"
#include "Editor.h"
#include "MeshTools.h"
#include "EditorScene.h"

#include <base/CCDirector.h>
#include <2d/CCCamera.h>
#include <3d/CCSprite3D.h>

#include <QMouseEvent>
#include <QKeyEvent>


USING_NS_CC;

namespace Editor
{

    Canvas3D::Canvas3D(QObject *parent, GLWidget *view)
        : Canvas(parent, view)
        , cameraMoveSpeed_(500.0f)
        , moveDirection_(DIR_NONE)
    {
        auto scene = Editor::instance()->getScene();
        scene->setFor3D(true);

        auto director = Director::getInstance();

        camera_ = scene->getDefaultCamera();
        float aspect = director->getWinSize().width / director->getWinSize().height;
        camera_->initPerspective(60, aspect, 1.0f, 10000.0f);
        camera_->setPosition3D(Vec3(0.0f, 500.0f, 500.0f));
        camera_->lookAt(Vec3::ZERO);

        drawNode_ = DrawNode3D::create();
        scene->addChild(drawNode_);

        Sprite3D *ground = createSquareModel(10000, 10000, "res/green.png", 100);
        //Sprite3D *ground = createGridModel(50, 50, 100, 100, "res/green.png");
        ground->setOpacity(254); //force draw in transparent pass.
        scene->addChild(ground);
    }

    void Canvas3D::onRootSet(cocos2d::Node *root)
    {
        Canvas::onRootSet(root);
    }

    void Canvas3D::onPopertyChange(PropertyParam &param)
    {

    }

    void Canvas3D::onMouseEvent(QMouseEvent *event)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            cocos2d::Point pt(event->x(), event->y());
            lastMousePosition_ = cocos2d::Director::getInstance()->convertToUI(pt);
        }
        else if(event->type() == QEvent::MouseButtonRelease)
        {

        }
        else if(event->type() == QEvent::MouseMove)
        {
            if(event->buttons() & Qt::RightButton)
            {
                cocos2d::Point pt(event->x(), event->y());
                pt = cocos2d::Director::getInstance()->convertToUI(pt);
                doCameraRotation(pt, lastMousePosition_);
                lastMousePosition_ = pt;
            }
        }
    }

    void Canvas3D::onWheelEvent(QWheelEvent * event)
    {
        QPoint angleDelta = event->angleDelta() / 8;

        float delta = angleDelta.y() / 180.0f * cameraMoveSpeed_;

        Vec3 direction;
        camera_->getNodeToParentTransform().getForwardVector(&direction);

        Vec3 position = camera_->getPosition3D() + direction * delta;
        camera_->setPosition3D(position);
    }

    void Canvas3D::onKeyEvent(QKeyEvent *event)
    {
        int dir = 0;
        switch(event->key())
        {
        case Qt::Key_Left:
            dir = DIR_LEFT;
            break;
        case Qt::Key_Right:
            dir = DIR_RIGHT;
            break;
        case Qt::Key_Up:
            dir = DIR_FRONT;
            break;
        case Qt::Key_Down:
            dir = DIR_BACK;
            break;
        default:
            break;
        }

        if(dir != DIR_NONE)
        {
            if(event->type() == QEvent::KeyPress)
            {
                moveDirection_ |= dir;
            }
            else if(event->type() == QEvent::KeyRelease)
            {
                moveDirection_ &= ~dir;
            }
        }
    }

    void Canvas3D::onResize(float width, float height)
    {

    }

    void Canvas3D::onTick(float dt)
    {
        if(moveDirection_ != DIR_NONE)
        {
            const Mat4 & transform = camera_->getNodeToParentTransform();
            Vec3 direction;
            if(moveDirection_ & DIR_LEFT)
            {
                transform.getLeftVector(&direction);
            }
            else if(moveDirection_ & DIR_RIGHT)
            {
                transform.getRightVector(&direction);
            }

            if(moveDirection_ & DIR_FRONT)
            {
                transform.getForwardVector(&direction);
                direction.y = 0.0f;
            }
            else if(moveDirection_ & DIR_BACK)
            {
                transform.getBackVector(&direction);
                direction.y = 0.0f;
            }

            direction.normalize();

            Vec3 position = camera_->getPosition3D() + direction * (dt * cameraMoveSpeed_);
            camera_->setPosition3D(position);
        }
    }

    void Canvas3D::drawSelectedRect()
    {
        drawNode_->clear();

        if(targetNode_)
        {
            AABB aabb = Sprite3D::getAABBRecursivelyImp(targetNode_);
            drawNode_->drawAABB(aabb, Color4F::WHITE);
        }
    }

    void Canvas3D::doCameraRotation(const cocos2d::Vec2 &newPt, const cocos2d::Vec2 &oldPt)
    {
        Vec2 delta = newPt - oldPt;

        Size winSize = Director::getInstance()->getWinSize();

        float dx = delta.x / winSize.width;
        float dy = delta.y / winSize.height;

        Vec3 rotation = camera_->getRotation3D();
        rotation.y -= dx * 180.0f;
        rotation.x += dy * 180.0f;
        camera_->setRotation3D(rotation);
    }
}
