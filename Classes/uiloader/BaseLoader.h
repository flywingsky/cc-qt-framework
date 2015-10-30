//
//  BaseLoader.h
//  Clover
//
//  Created by C218-pc on 15/6/26.
//
//

#ifndef Clover_BaseLoader_h
#define Clover_BaseLoader_h

#include "UIMacros.h"
#include <2d/CCNode.h>
#include <rapidjson/document.h>


#define DECLARE_LOADER(CLASS) \
    typedef this_class   base_class; \
    typedef CLASS        this_class; \
    virtual const char * getLoaderName() const { return #CLASS; }


class PropertyParam
{
public:
    PropertyParam(cocos2d::Node *_node,
                  const std::string & _name,
                  const rapidjson::Value & _value,
                  rapidjson::Value & _properties,
                  rapidjson::Value::AllocatorType & _allocator)
    : node(_node)
    , name(_name)
    , value(_value)
    , properties(_properties)
    , pAllocator(&_allocator)
    {}

    PropertyParam(cocos2d::Node *_node,
                  const std::string & _name,
                  const rapidjson::Value & _value,
                  rapidjson::Value & _properties)
    : node(_node)
    , name(_name)
    , value(_value)
    , properties(_properties)
    , pAllocator(nullptr)
    {}

    rapidjson::Value::AllocatorType& allocator(){ return *pAllocator; }
    
public:
    cocos2d::Node *                     node;
    const std::string &                 name;
    const rapidjson::Value &            value;
    rapidjson::Value &                  properties;
    rapidjson::Value::AllocatorType*    pAllocator;
};


class IBaseLoader
{
public:
    typedef IBaseLoader this_class;
    
    static const int Version;
    
    IBaseLoader(){}
    virtual ~IBaseLoader(){}
    
    virtual const char * getLoaderName() const { return "IBaseLoader"; }
    
    virtual cocos2d::Node * createObject(rapidjson::Value & config) = 0;
    virtual bool setProperty(PropertyParam &param) = 0;
    
    /** return false if can't upgrade.*/
    virtual bool upgradeProperty(rapidjson::Value & property, rapidjson::Value::AllocatorType & allocator){ return true; }
    

    virtual void trimProperty(rapidjson::Value & property, rapidjson::Value::AllocatorType & allocator){}
    
    /** return false if the property can't be changed.*/
    virtual bool hookPropertyChange(PropertyParam & param){ return true; }
};

#endif
