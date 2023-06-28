/**
  ******************************************************************************
  * @file    log.h
  * @author  JZY
  * @version V1.0.0
  * @date    2021-12-25
  * @brief   None
  ******************************************************************************
  * @attention
  *
  * None
  ******************************************************************************
  */

#ifndef __LOG_H
#define __LOG_H

#include <stdio.h>
#include <stdarg.h>
#include "config.h"


/* define marco LOG_LEVEL */
#ifndef LOG_LEVEL
#define LOG_LEVEL   LOG_DEBUG
#endif
/* define marco LOG_SAVE */
#ifndef LOG_SAVE
#define LOG_SAVE    0
#endif

typedef enum
{
    LOG_DEBUG = 0,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
}LOGLEVEL;

void LOG(LOGLEVEL level, const char* fun, const int line, const char* fmt, ...);

#define logprintf(level, fmt...)    LOG(level, __FUNCTION__, __LINE__, fmt)

#endif /************* __LOG_H *************/
