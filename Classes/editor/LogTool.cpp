//
//  LogTool.cpp
//  Editor
//
//  Created by C218-pc on 15/8/13.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "LogTool.h"
#include "LogEventDispatcher.h"

#include <platform/CCCommon.h>

int g_logPriorityFilter = LOG_PRIORITY_DEBUG;

bool g_logToConsole = true;

static const int MaxLogLength = 1024 * 4;

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <android/log.h>
android_LogPriority priority2android(int priority)
{
    switch(priority)
    {
        case LOG_PRIORITY_DEBUG:
            return ANDROID_LOG_DEBUG;
        case LOG_PRIORITY_INFO:
            return ANDROID_LOG_INFO;
        case LOG_PRIORITY_NOTICE:
            return ANDROID_LOG_INFO;
        case LOG_PRIORITY_WARN:
            return ANDROID_LOG_WARN;
        case LOG_PRIORITY_ERROR:
            return ANDROID_LOG_ERROR;
        case LOG_PRIORITY_FATAL:
            return ANDROID_LOG_FATAL;
        default:
            return ANDROID_LOG_VERBOSE;
    }
}
#endif

static void getLocalTime(struct tm & timeinfo)
{
    time_t rawtime;
    memset(&timeinfo, 0, sizeof(timeinfo));
    
    time(&rawtime);
    localtime_r(&rawtime, &timeinfo);
}

void EditorLog(int moduleLvl, int logLvl, const char * tag, const char *format, ...)
{
    assert(logLvl >= 0);
    
    if(logLvl < std::max(moduleLvl, g_logPriorityFilter))
    {
        return;
    }
    
    char message[MaxLogLength];
    int length = 0;
    
    va_list ap;
    va_start(ap, format);
    length = vsnprintf(message, MaxLogLength, format, ap);
    va_end(ap);

    if(length < 0)
    {
        return;
    }
    
    if(g_logToConsole)
    {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        android_LogPriority androidPriority = priority2android(logLvl);
        __android_log_write(androidPriority, tag, message);

#else
        struct tm timeinfo;
        getLocalTime(timeinfo);
        
        const char * prefix = LogPriorityString[std::min(logLvl, LOG_PRIORITY_FATAL)];
        
        fprintf(stdout, "[%2.2d:%2.2d:%2.2d][%s][%s]", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, prefix, tag);
        fputs(message, stdout);
        fputs("\n", stdout);
        fflush(stdout);
#endif
    }

    if(Editor::LogEventDispatcher::hasInstance())
    {
        Editor::LogEventDispatcher::instance()->dispatchLog(logLvl, tag, message);
    }
}
