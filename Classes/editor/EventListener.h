//
//  UIEventListener.h
//  Clover
//
//  Created by C218-pc on 15/7/2.
//
//

#ifndef Clover_UIEventListener_h
#define Clover_UIEventListener_h

#include <base/CCRef.h>
#include "EditorConfig.h"

namespace Editor
{

template<typename Method>
struct EventListener
{
    cocos2d::Ref *  target;
    Method          method;
    
    EventListener()
    : target(NULL)
    , method(NULL)
    {}
    
    EventListener(cocos2d::Ref *target, Method method)
    {
        this->target = target;
        this->method = method;
    }
    
    void set(cocos2d::Ref *target, Method method)
    {
        this->target = target;
        this->method = method;
    }
    
    void clear()
    {
        target = NULL;
        method = NULL;
    }
    
    operator bool()
    {
        return target != NULL && method != NULL;
    }
    
    bool operator == (const EventListener<Method> & other) const
    {
        return this->target == other.target &&
            this->method == other.method;
    }
};

template<typename Method>
EventListener<Method> makeListener(cocos2d::Ref* target, Method method)
{
    return EventListener<Method>(target, method);
}

} // end namespace Editor

#endif
