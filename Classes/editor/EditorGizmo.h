#ifndef EDITORGIZMO_H
#define EDITORGIZMO_H

#include <2d/CCNode.h>
#include <3d/CCPlane.h>

#include <functional>

namespace Editor
{

    class MouseEventDelegate
    {
    public:
        virtual ~MouseEventDelegate(){}

        virtual bool onMousePress(const cocos2d::Vec2 &pt) = 0;
        virtual void onMouseDrag(const cocos2d::Vec2 &pt, const cocos2d::Vec2 &last){}
        virtual void onMouseRelease(const cocos2d::Vec2 &pt){}
    };

    class GizmoNode : public cocos2d::Node, public MouseEventDelegate
    {
    public:
        typedef std::function<void(const cocos2d::Vec3 &)> PositionChangeListener;

        CREATE_FUNC(GizmoNode)

        GizmoNode();
        ~GizmoNode();

        virtual bool init() override;
        virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) override;

        bool onMousePress(const cocos2d::Vec2 &pt) override;
        void onMouseDrag(const cocos2d::Vec2 &pt, const cocos2d::Vec2 &last) override;
        void onMouseRelease(const cocos2d::Vec2 &pt) override;

    private:
        cocos2d::Node*  root_;
        cocos2d::Node*  intersectNode_;

        // the position is in world coordinate system.
        cocos2d::Vec3   srcOrigin_;
        cocos2d::Vec3   intersectAxis_;
        cocos2d::Vec3   intersectOrigin_;
        cocos2d::Plane  verticalPlane_;
    };

}

#endif // EDITORGIZMO_H
