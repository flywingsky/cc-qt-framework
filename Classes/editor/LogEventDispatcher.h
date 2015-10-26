//
//  LogEventListener.h
//  Editor
//
//  Created by C218-pc on 15/8/13.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Editor__LogEventListener__
#define __Editor__LogEventListener__

#include "EditorConfig.h"
#include "Singleton.h"
#include "EventListener.h"

#include <vector>

#if LOG_THEAD_SAFE
#include <pthread.h>
#endif

namespace Editor
{

typedef void(cocos2d::Ref::*SEL_LogEvent)(int priority, const char * tag, const char * log);

class LogEventDispatcher : public Singleton<LogEventDispatcher>
{
public:
    LogEventDispatcher();
    ~LogEventDispatcher();
    
    typedef EventListener<SEL_LogEvent> LogEventListener;
    
    void dispatchLog(int priority, const char * tag, const char * log);
    
    void subscribe(cocos2d::Ref *target, SEL_LogEvent method);
    void unsubscribe(cocos2d::Ref *target, SEL_LogEvent method);
    
    void lock();
    void unlock();
    
private:
    typedef std::vector<LogEventListener> Listeners;
    Listeners       m_listener;
    
#if LOG_THEAD_SAFE
    pthread_mutex_t m_mutex;
#endif
};

}

#endif /* defined(__Editor__LogEventListener__) */
