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
    pthread_mutex_init(&m_mutex, NULL);
#endif
}

LogEventDispatcher::~LogEventDispatcher()
{
#if LOG_THEAD_SAFE
    pthread_mutex_destroy(&m_mutex);
#endif
}

void LogEventDispatcher::lock()
{
#if LOG_THEAD_SAFE
    pthread_mutex_lock(&m_mutex);
#endif
}

void LogEventDispatcher::unlock()
{
#if LOG_THEAD_SAFE
    pthread_mutex_unlock(&m_mutex);
#endif
}

void LogEventDispatcher::dispatchLog(int priority, const char * tag, const char * log)
{
    lock();
    
    for(Listeners::iterator it = m_listener.begin();
        it != m_listener.end(); ++it)
    {
        (it->target->*(it->method))(priority, tag, log);
    }
    
    unlock();
}

void LogEventDispatcher::subscribe(cocos2d::Ref *target, SEL_LogEvent method)
{
    LogEventListener listener(target, method);
    
    lock();
    m_listener.push_back(listener);
    unlock();
}

void LogEventDispatcher::unsubscribe(cocos2d::Ref *target, SEL_LogEvent method)
{
    LogEventListener listener(target, method);
    
    lock();
    Listeners::iterator it = std::find(m_listener.begin(), m_listener.end(), listener);
    if(it != m_listener.end())
    {
        m_listener.erase(it);
    }
    unlock();
}

} //namespace Editor
