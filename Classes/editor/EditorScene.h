#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include <2d/CCScene.h>

namespace Editor
{

    class EditorScene : public cocos2d::Scene
    {
    public:
        CREATE_FUNC(EditorScene)

        EditorScene();
        ~EditorScene();

        void setFor3D(bool is3D);

    protected:
        void onProjectionChanged(cocos2d::EventCustom* event);

        bool    _isFor3D;
    };
}

#endif // EDITORSCENE_H
