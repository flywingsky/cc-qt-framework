#include "CCSprite3DLoader.h"
#include "../UIHelper.h"

#ifdef BUILD_EDITOR
#include "editor/EditorSprite3D.h"
typedef EditorSprite3D _Sprite3D;
#else
#include <3d/CCSprite3D.h>
typedef cocos2d::Sprite3D _Sprite3D;
#endif


DECLARE_PROPERTY_SETTER(_Sprite3D, model, load, std::string)

CCSprite3DLoader::CCSprite3DLoader()
{
#ifdef BUILD_EDITOR
    REGISTER_PROPERTY_SETTER(model);
#endif
}

cocos2d::Node* CCSprite3DLoader::createObject(rapidjson::Value & config)
{
    const rapidjson::Value &jpath = config["model"];
    if(jpath.IsString())
    {
        return _Sprite3D::create(jpath.GetString());
    }
    else
    {
        return _Sprite3D::create();
    }
}
