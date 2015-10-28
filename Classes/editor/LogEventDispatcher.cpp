//
//  LogEventListener.cpp
//  Editor
//
//  Created by C218-pc on 15/8/13.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "LogEventDispatcher.h"

IMPLEMENT_SINGLETON(Editor::LogEventDispatcher);

namespace Editor
{

LogEventDispatcher::LogEventDispatcher()
{
#if LOG_THEAD_SAFE
    pthread_mutex_init(&mutex_, NULL);
#endif
}

LogEventDispatcher::~LogEventDispatcher()
{
#if LOG_THEAD_SAFE
    pthread_mutex_destroy(&mutex_);
#endif
}

void LogEventDispatcher::lock()
{
#if LOG_THEAD_SAFE
    pthread_mutex_lock(&mutex_);
#endif
}

void LogEventDispatcher::unlock()
{
#if LOG_THEAD_SAFE
    pthread_mutex_unlock(&mutex_);
#endif
}

void LogEventDispatcher::dispatchLog(int priority, const char * tag, const char * log)
{
    lock();
    
    for(Listeners::iterator it = listener_.begin();
        it != listener_.end(); ++it)
    {
        (it->target->*(it->method))(priority, tag, log);
    }
    
    unlock();
}

void LogEventDispatcher::subscribe(cocos2d::Ref *target, SEL_LogEvent method)
{
    LogEventListener listener(target, method);
    
    lock();
    listener_.push_back(listener);
    unlock();
}

void LogEventDispatcher::unsubscribe(cocos2d::Ref *target, SEL_LogEvent method)
{
    LogEventListener listener(target, method);
    
    lock();
    Listeners::iterator it = std::find(listener_.begin(), listener_.end(), listener);
    if(it != listener_.end())
    {
        listener_.erase(it);
    }
    unlock();
}

} //namespace Editor
