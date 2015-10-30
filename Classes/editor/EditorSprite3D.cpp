#include "EditorSprite3D.h"

EditorSprite3D::EditorSprite3D()
{

}

EditorSprite3D::~EditorSprite3D()
{

}

/*static*/ EditorSprite3D* EditorSprite3D::create()
{
    EditorSprite3D *p = new (std::nothrow) EditorSprite3D();
    if(p->init())
    {
        p->autorelease();
        return p;
    }
    delete p;
    return nullptr;
}

/*static*/ EditorSprite3D* EditorSprite3D::create(const std::string &resource)
{
    EditorSprite3D *sprite = new (std::nothrow) EditorSprite3D();
    if (sprite && sprite->initWithFile(resource))
    {
        cocos2d::Size size = sprite->getBoundingBox().size;
        sprite->_contentSize.setSize(std::max(0.0f, size.width), std::max(0.0f, size.height));
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

bool EditorSprite3D::load(const std::string &resource)
{
    return initWithFile(resource);
}

