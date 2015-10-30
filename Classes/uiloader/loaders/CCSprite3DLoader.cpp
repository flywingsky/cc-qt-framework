#include "CCSprite3DLoader.h"

#include <3d/CCSprite3D.h>

#ifdef BUILD_EDITOR
#include "editor/EditorSprite3D.h"
#endif

CCSprite3DLoader::CCSprite3DLoader()
{

}

cocos2d::Node* CCSprite3DLoader::createObject(rapidjson::Value & config)
{
#ifdef BUILD_EDITOR
    return EditorSprite3D::create();
#else
    const rapidjson::Value &jpath = config["model"];
    if(jpath.IsString())
    {
        return cocos2d::Sprite3D::create(jpath.GetString());
    }
    else
    {
        return return cocos2d::Sprite3D::create();
    }
#endif
}

bool CCSprite3DLoader::setProperty(PropertyParam &pp)
{
#ifdef BUILD_EDITOR
    EditorSprite3D *sp = dynamic_cast<EditorSprite3D*>(pp.node);
#else
    cocos2d::Sprite3D *sp = dynamic_cast<cocos2d::Sprite3D*>(pp.node);
#endif
    CCAssert(sp, "CCSprite3DLoader::setProperty");

    if(pp.name == "model")
    {
#ifdef BUILD_EDITOR
        if(pp.value.IsString())
        {
            sp->load(pp.value.GetString());
        }
#endif
    }
    else
    {
        return base_class::setProperty(pp);
    }
}
