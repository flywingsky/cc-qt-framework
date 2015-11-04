#include "EditorGizmo.h"

#include "MeshTools.h"

USING_NS_CC;

namespace Editor
{
    GizmoNode::GizmoNode()
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

        return true;
    }
}

