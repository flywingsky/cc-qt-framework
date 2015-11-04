#include "EditorGizmo.h"

#include "MeshTools.h"

#include <2d/CCCamera.h>
#include <3d/CCSprite3D.h>

USING_NS_CC;

namespace Editor
{
    GizmoNode::GizmoNode()
        : root_(nullptr)
    {

    }

    GizmoNode::~GizmoNode()
    {

    }

    bool GizmoNode::init()
    {
        if(!Node::init())
        {
            return false;
        }

        root_ = Node::create();
        this->addChild(root_);

        float radius = 0.5f;
        float length = 10;

        Sprite3D *sprite;

        // x
        sprite = createCube(Vec3(length * radius, radius, radius), Color4B::RED);
        sprite->setPosition3D(Vec3((length + 1) * radius, radius, 0.0f));
        sprite->setForce2DQueue(true);
        root_->addChild(sprite);

        // y
        sprite = createCube(Vec3(radius, length * radius, radius), Color4B::GREEN);
        sprite->setPosition3D(Vec3(0.0f, (length + 2) * radius, 0.0f));
        sprite->setForce2DQueue(true);
        root_->addChild(sprite);

        // z
        sprite = createCube(Vec3(radius, radius, length * radius), Color4B::BLUE);
        sprite->setPosition3D(Vec3(0.0f, radius, (length + 1) * radius));
        sprite->setForce2DQueue(true);
        root_->addChild(sprite);
        return true;
    }

    void GizmoNode::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
    {
        const Camera *camera = Camera::getVisitingCamera();

        float distance = camera->getPosition3D().distance(root_->getPosition3D());
        float scale = distance / 50.0f;
        root_->setScale(scale);

        Node::visit(renderer, parentTransform, parentFlags);
    }
}

