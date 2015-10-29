//
//  EditorTools.cpp
//  Editor
//
//  Created by C218-pc on 15/7/14.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "EditorTools.h"

#include <QPoint>
#include <QPointF>
#include <QSize>
#include <QSizeF>
#include <QList>
#include <QMap>
#include <QRect>
#include <QRectF>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QColor>

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
    
    void json2tvalue(QVariant & output, const rapidjson::Value & input, int itype)
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
            output = QVariant(QString(input.GetString()));
        }
        else if(input.IsArray())
        {
            switch(itype)
            {
            case QVariant::Point:
            {
                assert(input.Size() == 2);
                output = QPoint(input[0u].GetDouble(), input[1].GetDouble());
                break;
            }
            case QVariant::PointF:
            {
                assert(input.Size() == 2);
                output = QPointF(input[0u].GetDouble(), input[1].GetDouble());
                break;
            }
            case QVariant::Size:
            {
                assert(input.Size() == 2);
                output = QSize(input[0u].GetDouble(), input[1].GetDouble());
                break;
            }
            case QVariant::SizeF:
            {
                assert(input.Size() == 2);
                output = QSizeF(input[0u].GetDouble(), input[1].GetDouble());
                break;
            }
            case QVariant::Rect:
            {
                assert(input.Size() == 4);
                output = QRect(input[0u].GetDouble(), input[1].GetDouble(), input[2].GetDouble(), input[3].GetDouble());
                break;
            }
            case QVariant::RectF:
            {
                assert(input.Size() == 4);
                output = QRectF(input[0u].GetDouble(), input[1].GetDouble(), input[2].GetDouble(), input[3].GetDouble());
                break;
            }
            case QVariant::Vector2D:
            {
                assert(input.Size() == 2);
                output = QVector2D(input[0u].GetDouble(), input[1].GetDouble());
                break;
            }
            case QVariant::Vector3D:
            {
                assert(input.Size() == 3);
                output = QVector3D(input[0u].GetDouble(), input[1].GetDouble(), input[2].GetDouble());
                break;
            }
            case QVariant::Vector4D:
            {
                assert(input.Size() == 4);
                output = QVector4D(input[0u].GetDouble(), input[1].GetDouble(), input[2].GetDouble(), input[3].GetDouble());
                break;
            }
            case QVariant::Color:
            {
                assert(input.Size() == 4);
                output = QColor(input[0u].GetDouble(), input[1].GetDouble(), input[2].GetDouble(), input[3].GetDouble());
                break;
            }
            case QVariant::StringList:
            {
                QStringList list;
                list.reserve(input.Size());

                for(rapidjson::Value::ConstValueIterator it = input.Begin(); it != input.End(); ++it)
                {
                    QString tmp(it->GetString());
                    list.append(tmp);
                }
                output = list;
                break;
            }
            case QVariant::List:
            default:
            {
                QList<QVariant> list;
                list.reserve(input.Size());

                for(rapidjson::SizeType i = 0; i < input.Size(); ++i)
                {
                    QVariant tmp;
                    json2tvalue(tmp, input[i], QVariant::Invalid);
                    list.append(tmp);
                }
                output = list;
                break;
            }
            }
        }
        else if(input.IsObject())
        {
            QMap<QString, QVariant> map;

            for(rapidjson::Value::ConstMemberIterator it= input.MemberBegin();
                it != input.MemberEnd(); ++it)
            {
                QString qstring(QLatin1String(it->name.GetString(), it->name.GetStringLength()));
                QVariant qvalue;
                json2tvalue(qvalue, it->value, QVariant::Invalid);

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
        case QVariant::StringList:
        {
            QStringList list = input.toStringList();

            output.SetArray();
            output.Reserve(list.size(), allocator);

            rapidjson::Value temp;
            for(QStringList::const_iterator it = list.begin(); it != list.end(); ++it)
            {
                QByteArray str = it->toUtf8();
                temp.SetString(str.data(), str.size(), allocator);
                output.PushBack(temp, allocator);
            }
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
            break;
        }
        case QVariant::Point:
        {
            QPoint pt = input.toPoint();

            output.SetArray();
            output.Reserve(2, allocator);

            output.PushBack(pt.x(), allocator);
            output.PushBack(pt.y(), allocator);
            break;
        }
        case QVariant::PointF:
        {
            QPointF pt = input.toPointF();

            output.SetArray();
            output.Reserve(2, allocator);

            output.PushBack(pt.x(), allocator);
            output.PushBack(pt.y(), allocator);
            break;
        }
        case QVariant::Size:
        {
            QSize pt = input.toSize();

            output.SetArray();
            output.Reserve(2, allocator);

            output.PushBack(pt.width(), allocator);
            output.PushBack(pt.height(), allocator);
            break;
        }
        case QVariant::SizeF:
        {
            QSizeF pt = input.toSizeF();

            output.SetArray();
            output.Reserve(2, allocator);

            output.PushBack(pt.width(), allocator);
            output.PushBack(pt.height(), allocator);
            break;
        }
        case QVariant::Rect:
        {
            QRect pt = input.toRect();

            output.SetArray();
            output.Reserve(4, allocator);

            output.PushBack(pt.x(), allocator);
            output.PushBack(pt.y(), allocator);
            output.PushBack(pt.width(), allocator);
            output.PushBack(pt.height(), allocator);
            break;
        }
        case QVariant::RectF:
        {
            QRectF pt = input.toRectF();

            output.SetArray();
            output.Reserve(4, allocator);

            output.PushBack(pt.x(), allocator);
            output.PushBack(pt.y(), allocator);
            output.PushBack(pt.width(), allocator);
            output.PushBack(pt.height(), allocator);
            break;
        }
        case QVariant::Vector2D:
        {
            QVector2D pt = input.value<QVector2D>();

            output.SetArray();
            output.Reserve(2, allocator);

            output.PushBack(pt.x(), allocator);
            output.PushBack(pt.y(), allocator);
            break;
        }
        case QVariant::Vector3D:
        {
            QVector3D pt = input.value<QVector3D>();

            output.SetArray();
            output.Reserve(3, allocator);

            output.PushBack(pt.x(), allocator);
            output.PushBack(pt.y(), allocator);
            output.PushBack(pt.z(), allocator);
            break;
        }
        case QVariant::Vector4D:
        {
            QVector4D pt = input.value<QVector4D>();

            output.SetArray();
            output.Reserve(4, allocator);

            output.PushBack(pt.x(), allocator);
            output.PushBack(pt.y(), allocator);
            output.PushBack(pt.z(), allocator);
            output.PushBack(pt.w(), allocator);
            break;
        }
        case QVariant::Color:
        {
            QColor pt = input.value<QColor>();

            output.SetArray();
            output.Reserve(4, allocator);

            output.PushBack(pt.red(), allocator);
            output.PushBack(pt.green(), allocator);
            output.PushBack(pt.blue(), allocator);
            output.PushBack(pt.alpha(), allocator);
            break;
        }
        default:
        {
            output.SetNull();
            assert(false && "shuldn't execute to here.");
        }
        }
    }
} // end namespace Editor
