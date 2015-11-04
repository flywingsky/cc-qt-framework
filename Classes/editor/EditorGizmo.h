#ifndef EDITORGIZMO_H
#define EDITORGIZMO_H

#include <2d/CCNode.h>

namespace Editor
{

    class GizmoNode : public cocos2d::Node
    {
    public:
        CREATE_FUNC(GizmoNode)

        GizmoNode();
        ~GizmoNode();

        virtual bool init() override;
        virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) override;
    private:
        cocos2d::Node*  root_;
    };

}

#endif // EDITORGIZMO_H
