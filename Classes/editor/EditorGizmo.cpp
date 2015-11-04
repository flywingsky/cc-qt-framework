#include "EditorGizmo.h"

#include "MeshTools.h"

#include <2d/CCCamera.h>
#include <3d/CCSprite3D.h>
#include <3d/CCRay.h>

USING_NS_CC;

namespace Editor
{
    Ray screenPtToWorldRay(const Vec2 &pt, Camera *camera)
    {
        Vec3 rayOrg = Vec3::ZERO;
        camera->getNodeToWorldTransform().transformPoint(&rayOrg);

        Vec3 pn = camera->unprojectGL(Vec3(pt.x, pt.y, -1.0f));
        Vec3 rayDir = pn - rayOrg;

        return Ray(rayOrg, rayDir);
    }

    GizmoNode::GizmoNode()
        : root_(nullptr)
        , intersectNode_(nullptr)
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
        sprite->setName("x");
        sprite->setPosition3D(Vec3((length + 1) * radius, radius, 0.0f));
        sprite->setForce2DQueue(true);
        root_->addChild(sprite);

        // y
        sprite = createCube(Vec3(radius, length * radius, radius), Color4B::GREEN);
        sprite->setName("y");
        sprite->setPosition3D(Vec3(0.0f, (length + 2) * radius, 0.0f));
        sprite->setForce2DQueue(true);
        root_->addChild(sprite);

        // z
        sprite = createCube(Vec3(radius, radius, length * radius), Color4B::BLUE);
        sprite->setName("z");
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

    bool GizmoNode::onMousePress(const cocos2d::Vec2 &pt)
    {
        Ray ray = screenPtToWorldRay(pt, Camera::getDefaultCamera());
        float minDistance = Camera::getDefaultCamera()->getFarPlane();

        intersectNode_ = nullptr;

        for(Node *child : root_->getChildren())
        {
            Sprite3D *sprite = dynamic_cast<Sprite3D*>(child);

            AABB aabb = sprite->getAABBRecursively();
            float distance;
            if(ray.intersects(aabb, &distance) && distance < minDistance)
            {
                intersectNode_ = sprite;
                minDistance = distance;
            }
        }

        return intersectNode_ != nullptr;
    }

    void GizmoNode::onMouseDrag(const cocos2d::Vec2 &pt, const cocos2d::Vec2 &last)
    {
        CCAssert(intersectNode_ != nullptr, "onMouseDrag");


    }

    void GizmoNode::onMouseRelease(const cocos2d::Vec2 &pt)
    {
        intersectNode_ = nullptr;
    }
}

