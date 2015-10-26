//
//  EditorTools.h
//  Editor
//
//  Created by C218-pc on 15/7/14.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Editor__EditorTools__
#define __Editor__EditorTools__

#include <QVariant>
#include <rapidjson/document.h>

namespace Editor
{
    
    bool clone_value(rapidjson::Value & output, const rapidjson::Value & input, rapidjson::Value::AllocatorType & allocator);
    
    void json2tvalue(QVariant & output, const rapidjson::Value & input);
    void tvalue2json(rapidjson::Value & output, const QVariant & input, rapidjson::Value::AllocatorType & allocator);
    
}

#endif /* defined(__Editor__EditorTools__) */
