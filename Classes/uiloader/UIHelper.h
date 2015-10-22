//
//  UIHelper.h
//  Editor
//
//  Created by C218-pc on 15/7/14.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Editor__UIHelper__
#define __Editor__UIHelper__

#include <ccTypes.h>
#include <math/CCGeometry.h>

#include <rapidjson/document.h>

NS_CC_BEGIN
class CCNode;
NS_CC_END

namespace helper
{
    template<typename T>
    bool parseNumber(const rapidjson::Value & val, T & ret)
    {
        if(val.IsInt())         ret = (T)val.GetInt();
        else if(val.IsUint())   ret = (T)val.GetUint();
        else if(val.IsInt64())  ret = (T)val.GetInt64();
        else if(val.IsUint64()) ret = (T)val.GetUint64();
        else if(val.IsDouble()) ret = (T)val.GetDouble();
        else return false;
        
        return true;
    }
    
    template<typename T>
    T parseNumber(const rapidjson::Value & val)
    {
        T ret;
        if(!parseInt(val, ret)) ret = T();
        return ret;
    }
    
    template<typename T>
    bool parseNumberArray(const rapidjson::Value & value, T * p, size_t n)
    {
        if(!value.IsArray() || value.Size() < n) return false;
        
        for(typename rapidjson::SizeType i = 0; i < value.Size(); ++i)
        {
            if(!parseNumber(value[i], p[i])) return false;
        }
        return true;
    }
    
    inline bool parseValue(const rapidjson::Value & value, cocos2d::CCPoint & pt)
    {
        return parseNumberArray(value, (float*)&pt, 2);
    }
    
    inline bool parseValue(const rapidjson::Value & value, cocos2d::CCSize & sz)
    {
        return parseNumberArray(value, (float*)&sz, 2);
    }
    
    inline bool parseValue(const rapidjson::Value & value, cocos2d::CCRect & rc)
    {
        return parseNumberArray(value, (float*)&rc, 4);
    }
    
    inline bool parseValue(const rapidjson::Value & value, cocos2d::ccColor3B & cr)
    {
        return parseNumberArray(value, (GLubyte*)&cr, 3);
    }
    
    inline bool parseValue(const rapidjson::Value & value, cocos2d::ccColor4B & cr)
    {
        return parseNumberArray(value, (GLubyte*)&cr, 4);
    }
}

bool openJsonFile(const std::string & filename, rapidjson::Document & document);
bool saveJsonFile(const std::string & filename, rapidjson::Document & document);

void scaleNodeToSize(cocos2d::CCNode *node, const cocos2d::CCSize & size);

#endif /* defined(__Editor__UIHelper__) */
