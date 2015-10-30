//
//  BaseLoader.cpp
//  Editor
//
//  Created by C218-pc on 15/8/27.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "BaseLoader.h"

/*static*/ const int IBaseLoader::Version = 1;

#if 1 //TODO remove the following code, use loaders/LoaderAddin.cpp instead.
#include "UILoader.h"
#include "loaders/CCNodeLoader.h"
#include "loaders/CCSpriteLoader.h"
#include "loaders/CCSprite3DLoader.h"
void loadDefaultLoaders(UILoader *ld)
{
    ld->registerLoader("Node", new CCNodeLoader());
    ld->registerLoader("Sprite", new CCSpriteLoader());
    ld->registerLoader("Sprite3D", new CCSprite3DLoader());
}
#endif
