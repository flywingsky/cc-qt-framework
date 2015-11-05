#include "EditorCanvas3D.h"

#include "DrawNode3D.h"
#include "Editor.h"
#include "MeshTools.h"
#include "EditorScene.h"
#include "EditorGizmo.h"

#include <base/CCDirector.h>
#include <2d/CCCamera.h>
#include <2d/CCLight.h>
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
        , mouseListener_(nullptr)
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

        Vec3 direction(0, -1, -1);
        direction.normalize();
        DirectionLight *light = DirectionLight::create(direction, Color3B(150, 150, 150));
        scene->addChild(light);

        AmbientLight *light2 = AmbientLight::create(Color3B(80, 80, 80));
        scene->addChild(light2);

//        Sprite3D *test = createCube(Vec3(100, 100, 100), Color4B::WHITE);
//        test->setForce2DQueue(true);
//        scene->addChild(test);

        gizmo_ = GizmoNode::create();
        gizmo_->setPositionChangedListener(std::bind(&Canvas3D::onNodePositionChange, this, std::placeholders::_1));
        gizmo_->setVisible(false);
        //gizmo_->setScale(100);
        scene->addChild(gizmo_);
    }

    void Canvas3D::onRootSet(cocos2d::Node *root)
    {
        Canvas::onRootSet(root);
    }

    void Canvas3D::onTargetSet(cocos2d::Node *target)
    {
        targetNode_ = target;

        gizmo_->setVisible(target != nullptr);
        gizmo_->setTarget(target);

        drawSelectedRect();
    }

    void Canvas3D::onPopertyChange(PropertyParam &param)
    {

    }

    void Canvas3D::onMouseEvent(QMouseEvent *event)
    {
        cocos2d::Point pt(event->x(), event->y());
        pt = cocos2d::Director::getInstance()->convertToUI(pt);

        if(event->type() == QEvent::MouseButtonPress)
        {
            if(event->buttons() & Qt::LeftButton)
            {
                if(gizmo_->onMousePress(pt))
                {
                    mouseListener_ = gizmo_;
                }
            }
        }
        else if(event->type() == QEvent::MouseButtonRelease)
        {
            if(event->buttons() & Qt::LeftButton)
            {
                if(mouseListener_ != nullptr)
                {
                    mouseListener_->onMouseRelease(pt);
                    mouseListener_ = nullptr;
                }
            }
        }
        else if(event->type() == QEvent::MouseMove)
        {
            if(event->buttons() & Qt::LeftButton)
            {
                if(mouseListener_ != nullptr)
                {
                    mouseListener_->onMouseDrag(pt, lastMousePosition_);
                }
            }
            else if(event->buttons() & Qt::RightButton)
            {
                doCameraRotation(pt, lastMousePosition_);
            }
        }

        lastMousePosition_ = pt;
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
        case Qt::Key_A:
            dir = DIR_LEFT;
            break;
        case Qt::Key_Right:
        case Qt::Key_D:
            dir = DIR_RIGHT;
            break;
        case Qt::Key_Up:
        case Qt::Key_W:
            dir = DIR_FRONT;
            break;
        case Qt::Key_Down:
        case Qt::Key_S:
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

    void Canvas3D::onNodePositionChange(const cocos2d::Vec3 &position)
    {
        Vec3 temp;
        targetNode_->getParent()->getWorldToNodeTransform().transformPoint(position, &temp);
        targetNode_->setPosition3D(temp);

        gizmo_->setGlobalPosition(position);

        drawSelectedRect();
    }
}
