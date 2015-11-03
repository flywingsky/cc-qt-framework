//
//  MeshTools.h
//  demo
//
//  Created by C218-pc on 15/10/14.
//
//

#ifndef __demo__MeshTools__
#define __demo__MeshTools__

#include <3d/CCSprite3D.h>

namespace Editor
{
    cocos2d::Sprite3D* createSquareModel(float width, float height, const std::string & texture, float repeatStep=0.0f);

    cocos2d::Sprite3D* createGridModel(int rows, int cols,
                                       float gridWidth, float gridHeight,
                                       const std::string &texture);
}

#endif /* defined(__demo__MeshTools__) */
