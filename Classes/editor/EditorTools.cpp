//
//  EditorTools.cpp
//  Editor
//
//  Created by C218-pc on 15/7/14.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "EditorTools.h"

namespace Editor
{
    bool clone_value(rapidjson::Value & output, const rapidjson::Value & input, rapidjson::Value::AllocatorType & allocator)
    {
        if(&output == &input)
        {
            return false;
        }
        
        if(input.IsNull())
        {
            output.SetNull();
        }
        else if(input.IsInt64())
        {
            output.SetInt(input.GetInt64());
        }
        else if(input.IsInt())
        {
            output.SetInt(input.GetInt());
        }
        else if(input.IsDouble())
        {
            output.SetDouble(input.GetDouble());
        }
        else if(input.IsBool())
        {
            output.SetBool(input.GetBool());
        }
        else if(input.IsString())
        {
            output.SetString(input.GetString(), input.GetStringLength(), allocator);
        }
        else if(input.IsArray())
        {
            rapidjson::Value temp;
            output.SetArray();
            output.Reserve(input.Size(), allocator);
            for(rapidjson::SizeType i = 0; i < input.Size(); ++i)
            {
                clone_value(temp, input[i], allocator);
                output.PushBack(temp, allocator);
            }
        }
        else if(input.IsObject())
        {
            output.SetObject();
            
            rapidjson::Value key, val;
            for(rapidjson::Value::ConstMemberIterator it = input.MemberBegin();
                it != input.MemberEnd(); ++it)
            {
                clone_value(key, it->name, allocator);
                clone_value(val, it->value, allocator);
                
                output.AddMember(key, val, allocator);
            }
        }
        else
        {
            assert(false && "shuldn't execute to here.");
            return false;
        }
        return true;
    }
    
    void json2tvalue(QVariant & output, const rapidjson::Value & input)
    {
        if(input.IsNull())
        {
            output.clear();
        }
        else if(input.IsInt64())
        {
            output = QVariant(input.GetInt64());
        }
        else if(input.IsInt())
        {
            output = QVariant(input.GetInt());
        }
        else if(input.IsDouble())
        {
            output = QVariant(input.GetDouble());
        }
        else if(input.IsBool())
        {
            output = QVariant(input.GetBool());
        }
        else if(input.IsString())
        {
            output = QVariant(QLatin1String(input.GetString(), input.GetStringLength()));
        }
        else if(input.IsArray())
        {
            QList<QVariant> list;
            list.reserve(input.Size());

            for(rapidjson::SizeType i = 0; i < input.Size(); ++i)
            {
                QVariant tmp;
                json2tvalue(tmp, input[i]);
                list.append(tmp);
            }

            output = list;
        }
        else if(input.IsObject())
        {
            QMap<QString, QVariant> map;

            for(rapidjson::Value::ConstMemberIterator it= input.MemberBegin();
                it != input.MemberEnd(); ++it)
            {
                QString qstring(QLatin1String(it->name.GetString(), it->name.GetStringLength()));
                QVariant qvalue;
                json2tvalue(qvalue, it->value);

                map.insert(qstring, qvalue);
            }

            output = map;
        }
        else
        {
            output.clear();
            assert(false && "shouldn't execute to here.");
        }
    }
    
    void tvalue2json(rapidjson::Value & output, const QVariant & input, rapidjson::Value::AllocatorType & allocator)
    {
        switch(input.type())
        {
        case QVariant::Invalid:
        {
            output.SetNull();
            break;
        }
        case QVariant::Bool:
        {
            output.SetBool(input.toBool());
            break;
        }
        case QVariant::Int:
        {
            output.SetInt64(input.toInt());
            break;
        }
        case QVariant::LongLong:
        {
            output.SetInt64(input.toLongLong());
            break;
        }
        case QVariant::Double:
        {
            output.SetDouble(input.toDouble());
            break;
        }
        case QVariant::String:
        {
            QByteArray str = input.toString().toUtf8();
            output.SetString(str.data(), str.size(), allocator);
            break;
        }
        case QVariant::List:
        {
            QList<QVariant> list = input.toList();

            output.SetArray();
            output.Reserve(list.size(), allocator);

            rapidjson::Value temp;
            for(QList<QVariant>::const_iterator it = list.begin(); it != list.end(); ++it)
            {
                tvalue2json(temp, *it, allocator);
                output.PushBack(temp, allocator);
            }
            break;
        }
        case QVariant::Map:
        {
            output.SetObject();
            rapidjson::Value tempK, tempV;

            QMap<QString, QVariant> qmap = input.toMap();
            for(QMap<QString, QVariant>::const_iterator it = qmap.begin(); it != qmap.end(); ++it)
            {
                tvalue2json(tempK, it.key(), allocator);
                tvalue2json(tempV, it.value(), allocator);
                output.AddMember(tempK, tempV, allocator);
            }
        }
        default:
        {
            output.SetNull();
            assert(false && "shuldn't execute to here.");
        }
        }
    }
} // end namespace Editor
