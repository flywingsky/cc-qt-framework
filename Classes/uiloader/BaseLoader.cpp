//
//  BaseLoader.cpp
//  Editor
//
//  Created by C218-pc on 15/8/27.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "BaseLoader.h"

/*static*/ const int IBaseLoader::Version = 1;

IBaseLoader::IBaseLoader()
{

}

IBaseLoader::~IBaseLoader()
{

}

bool IBaseLoader::setProperty(cocos2d::Node *node, const std::string &name, const rapidjson::Value &value)
{
    auto it = setters_.find(name);
    if(it != setters_.end())
    {
        (it->second)(node, value);
        return true;
    }
    return false;
}

void IBaseLoader::registerPropertySetter(const std::string &propertyName, PropertySetter method)
{
    setters_[propertyName] = method;
}
