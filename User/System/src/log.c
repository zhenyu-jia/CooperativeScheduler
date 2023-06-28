/**
  ******************************************************************************
  * @file    log.c
  * @author  JZY
  * @version V1.0.0
  * @date    2021-12-25
  * @brief   一个简易的日志框架
  ******************************************************************************
  * @attention
  *
  * None
  ******************************************************************************
  */

#include "log.h"


__attribute__((weak)) void LOGSave(void);

#ifdef _DEBUG
static char* LOGLevelGet(LOGLEVEL level)
{
    if(level == LOG_DEBUG)
    {
        return "DEBUG";
    }
    else if (level == LOG_INFO)
    {
        return " INFO";
    }
    else if (level == LOG_WARN)
    {
        return " WARN";
    }
    else if (level == LOG_ERROR)
    {
        return "ERROR";
    }
    else
    {
        return "UNKNOW";
    }
}
#endif  /* _DEBUG */

void LOG(LOGLEVEL level, const char* fun, const int line, const char* fmt, ...)
{
#ifdef _DEBUG
    
    va_list arg;
    va_start(arg, fmt);
    
    char buf[1 + vsnprintf(NULL, 0, fmt, arg)];
    
    vsnprintf(buf, sizeof(buf), fmt, arg);
    va_end(arg);
    if((int)(level) >= (int)(LOG_LEVEL))
    {
        printf("[%s] [Fun:%s Line:%d] %s\r\n", LOGLevelGet(level), fun, line, buf);
        
#if(LOG_SAVE)
        // todo 存储日志
        LOGSave();
#endif  /* LOG_SAVE */
    }
#endif  /* _DEBUG */
}

__attribute__((weak)) void LOGSave(void)
{
}
