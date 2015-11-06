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


void operator >> (const rapidjson::Value &value, bool &v)
{
    if(value.IsBool())
    {
        v = value.GetBool();
    }
    else
    {
        v = false;
    }
}

void operator >> (const rapidjson::Value &value, int8_t &v)
{
    if(value.IsInt())
    {
        v = (int8_t)value.GetInt();
    }
    else
    {
        v = 0;
    }
}

void operator >> (const rapidjson::Value &value, uint8_t &v)
{
    if(value.IsUint())
    {
        v = (uint8_t)value.GetUint();
    }
    else
    {
        v = 0;
    }
}

void operator >> (const rapidjson::Value &value, int16_t &v)
{
    if(value.IsInt())
    {
        v = (int16_t)value.GetInt();
    }
    else
    {
        v = 0;
    }
}

void operator >> (const rapidjson::Value &value, uint16_t &v)
{
    if(value.IsUint())
    {
        v = (uint16_t)value.GetUint();
    }
    else
    {
        v = 0;
    }
}

void operator >> (const rapidjson::Value &value, int &v)
{
    if(value.IsInt())
    {
        v = value.GetInt();
    }
    else
    {
        v = 0;
    }
}

void operator >> (const rapidjson::Value &value, unsigned int &v)
{
    if(value.IsUint())
    {
        v = value.GetUint();
    }
    else
    {
        v = 0;
    }
}

void operator >> (const rapidjson::Value &value, int64_t &v)
{
    if(value.IsInt64())
    {
        v = value.GetInt64();
    }
    else
    {
        v = 0;
    }
}

void operator >> (const rapidjson::Value &value, uint64_t &v)
{
    if(value.IsUint64())
    {
        v = value.GetUint64();
    }
    else
    {
        v = 0;
    }
}

void operator >> (const rapidjson::Value &value, float &v)
{
    if(value.IsDouble())
    {
        v = value.GetDouble();
    }
    else
    {
        v = 0.0f;
    }
}

void operator >> (const rapidjson::Value &value, double &v)
{
    if(value.IsDouble())
    {
        v = value.GetDouble();
    }
    else
    {
        v = 0.0;
    }
}

void operator >> (const rapidjson::Value &value, std::string &v)
{
    if(value.IsString())
    {
        v.assign(value.GetString(), value.GetStringLength());
    }
    else
    {
        v.clear();
    }
}

void operator >> (const rapidjson::Value &value, cocos2d::Size &v)
{
    if(value.IsArray() && value.Size() >= 2)
    {
        value[0U] >> v.width;
        value[1] >> v.height;
    }
    else
    {
        v = cocos2d::Size::ZERO;
    }
}

void operator >> (const rapidjson::Value &value, cocos2d::Rect &v)
{
    if(value.IsArray() && value.Size() >= 4)
    {
        value[0U] >> v.origin.x;
        value[1] >> v.origin.y;
        value[2] >> v.size.width;
        value[3] >> v.size.height;
    }
    else
    {
        v = cocos2d::Rect::ZERO;
    }
}

void operator >> (const rapidjson::Value &value, cocos2d::Vec2 &v)
{
    if(value.IsArray() && value.Size() >= 2)
    {
        value[0U] >> v.x;
        value[1] >> v.y;
    }
    else
    {
        v = cocos2d::Vec2::ZERO;
    }
}

void operator >> (const rapidjson::Value &value, cocos2d::Vec3 &v)
{
    if(value.IsArray() && value.Size() >= 3)
    {
        value[0U] >> v.x;
        value[1] >> v.y;
        value[2] >> v.z;
    }
    else
    {
        v = cocos2d::Vec3::ZERO;
    }
}

void operator >> (const rapidjson::Value &value, cocos2d::Vec4 &v)
{
    if(value.IsArray() && value.Size() >= 4)
    {
        value[0U] >> v.x;
        value[1] >> v.y;
        value[2] >> v.z;
        value[3] >> v.w;
    }
    else
    {
        v = cocos2d::Vec4::ZERO;
    }
}

void operator >> (const rapidjson::Value &value, cocos2d::Quaternion &v)
{
    if(value.IsArray() && value.Size() >= 4)
    {
        value[0U] >> v.x;
        value[1] >> v.y;
        value[2] >> v.z;
        value[3] >> v.w;
    }
    else
    {
        v = cocos2d::Quaternion::ZERO;
    }
}

void operator >> (const rapidjson::Value &value, cocos2d::Color3B &v)
{
    if(value.IsArray() && value.Size() >= 3)
    {
        value[0U] >> v.r;
        value[1] >> v.g;
        value[2] >> v.b;
    }
    else
    {
        v = cocos2d::Color3B::BLACK;
    }
}

void operator >> (const rapidjson::Value &value, cocos2d::Color4B &v)
{
    if(value.IsArray() && value.Size() >= 4)
    {
        value[0U] >> v.r;
        value[1] >> v.g;
        value[2] >> v.b;
        value[3] >> v.a;
    }
    else
    {
        v = cocos2d::Color4B::BLACK;
    }
}

void operator >> (const rapidjson::Value &value, cocos2d::Color4F &v)
{
    if(value.IsArray() && value.Size() >= 4)
    {
        value[0U] >> v.r;
        value[1] >> v.g;
        value[2] >> v.b;
        value[3] >> v.a;
    }
    else
    {
        v = cocos2d::Color4F::BLACK;
    }
}

void operator >> (const rapidjson::Value &value, cocos2d::BlendFunc &v)
{
    if(value.IsArray() && value.Size() >= 2)
    {
        value[0u] >> v.src;
        value[1] >> v.dst;
    }
    else
    {
        v.src = 0;
        v.dst = 0;
    }
}
