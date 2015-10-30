#include "EditorSprite3D.h"

EditorSprite3D::EditorSprite3D()
{

}

EditorSprite3D::~EditorSprite3D()
{

}

/*static*/ EditorSprite3D* EditorSprite3D::create()
{
    EditorSprite3D *p = new EditorSprite3D();
    if(p->init())
    {
        p->autorelease();
        return p;
    }
    delete p;
    return nullptr;
}

bool EditorSprite3D::load(const std::string &resource)
{
    return initWithFile(resource);
}

