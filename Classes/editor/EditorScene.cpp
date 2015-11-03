#include "EditorScene.h"

#include <base/CCDirector.h>
#include <base/CCEventDispatcher.h>
#include <base/CCEventListenerCustom.h>
#include <2d/CCCamera.h>

USING_NS_CC;

namespace Editor
{

    EditorScene::EditorScene()
        : _isFor3D(false)
    {
        // disable origin projection change event.
        Director::getInstance()->getEventDispatcher()->removeEventListener(_event);
        _event->release();

        _event = Director::getInstance()->getEventDispatcher()->addCustomEventListener(Director::EVENT_PROJECTION_CHANGED, std::bind(&EditorScene::onProjectionChanged, this, std::placeholders::_1));
        _event->retain();
    }

    EditorScene::~EditorScene()
    {

    }

    void EditorScene::setFor3D(bool is3D)
    {
        _isFor3D = is3D;
        if(!_isFor3D && _defaultCamera)
        {
            _defaultCamera->initDefault();
        }
    }

    void EditorScene::onProjectionChanged(cocos2d::EventCustom* event)
    {
        if (_defaultCamera)
        {
            if(_isFor3D)
            {
                auto size = Director::getInstance()->getWinSize();
                float aspect = size.width / size.height;
                _defaultCamera->initPerspective(60, aspect, 1.0f, 10000.0f);
            }
            else
            {
                _defaultCamera->initDefault();
            }
        }
    }
}
