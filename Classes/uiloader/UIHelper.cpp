//
//  UIHelper.cpp
//  Editor
//
//  Created by C218-pc on 15/7/14.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "UIHelper.h"

#include <base/ccMacros.h>
#include <platform/CCFileUtils.h>
#include <2d/CCNode.h>

#include <rapidjson/prettywriter.h>
#include <rapidjson/filestream.h>
#include <errno.h>
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////

static void formatRowCol(const char * str, int offset, int & row, int & col)
{
    row = col = 1;
    
    while(*str && offset-- > 0)
    {
        if(*str++ == '\n')
        {
            ++row;
            col = 0;
        }
        ++col;
    }
}

bool openJsonFile(const std::string & filename, rapidjson::Document & document)
{
    std::string data = cocos2d::FileUtils::getInstance()->getStringFromFile(filename);
    if(data.empty())
    {
        CCLOGERROR("Failed to open file %s", filename.c_str());
        return false;
    }

    document.Parse<0>(data.c_str());
    if(document.HasParseError())
    {
        int r, c;
        formatRowCol(data.c_str(), (int)document.GetErrorOffset(), r, c);
        
        CCLOGERROR("Failed to parse json file '%s'(%d: %d): %s",
                   filename.c_str(), r, c, document.GetParseError());
        return false;
    }

    return true;
}

bool saveJsonFile(const std::string & filename, rapidjson::Document & document)
{
    FILE *fp = fopen(filename.c_str(), "w");
    if(NULL == fp)
    {
        CCLOGERROR("Failed to open file '%s' for write. code: %d", filename.c_str(), int(errno));
        return false;
    }
    rapidjson::FileStream stream(fp);
    
    rapidjson::PrettyWriter<rapidjson::FileStream> writer(stream, &document.GetAllocator());
    document.Accept(writer);

    fputc('\n', fp);
    fclose(fp);
    return true;
}

void scaleNodeToSize(cocos2d::Node *node, const cocos2d::Size & size)
{
    const cocos2d::Size & nodeSize = node->getContentSize();
    
    float x = 1.0f, y = 1.0f;
    if(size.width > 0 && nodeSize.width > 0)
        x = size.width / nodeSize.width;
    
    if(size.height > 0 && nodeSize.height > 0)
        y = size.height / nodeSize.height;
    
    node->setScaleX(x);
    node->setScaleY(y);
}
