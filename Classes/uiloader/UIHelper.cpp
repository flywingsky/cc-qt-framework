//
//  UIHelper.cpp
//  Editor
//
//  Created by C218-pc on 15/7/14.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "UIHelper.h"

#include <ccMacros.h>
#include <platform/CCFileUtils.h>
#include <base_nodes/CCNode.h>

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
    unsigned long size;
    char * pData = (char *)cocos2d::CCFileUtils::sharedFileUtils()->getFileData(filename.c_str(), "r", &size);
    if(pData == NULL)
    {
        CCLOGERROR("Failed to open file %s", filename.c_str());
        return false;
    }
    if(pData[size - 1] != '\n')
    {
        delete [] pData;
        CCLOGERROR("Invalid json file '%s', the file must has a blank line at end",
                   filename.c_str());
        return false;
    }
    pData[size - 1] = '\0';
    
    document.Parse<0>(pData);
    
    if(document.HasParseError())
    {
        int r, c;
        formatRowCol(pData, (int)document.GetErrorOffset(), r, c);
        
        CCLOGERROR("Failed to parse json file '%s'(%d: %d): %s",
                   filename.c_str(), r, c, document.GetParseError());
    }
    
    delete [] pData;
    return !document.HasParseError();
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

void scaleNodeToSize(cocos2d::CCNode *node, const cocos2d::CCSize & size)
{
    const cocos2d::CCSize & nodeSize = node->getContentSize();
    
    float x = 1.0f, y = 1.0f;
    if(size.width > 0 && nodeSize.width > 0)
        x = size.width / nodeSize.width;
    
    if(size.height > 0 && nodeSize.height > 0)
        y = size.height / nodeSize.height;
    
    node->setScaleX(x);
    node->setScaleY(y);
}
