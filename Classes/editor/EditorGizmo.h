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
    };

}

#endif // EDITORGIZMO_H
