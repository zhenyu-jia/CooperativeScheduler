/**
  ******************************************************************************
  * @file    config.c
  * @author  JZY
  * @version V1.0.0
  * @date    2021-12-25
  * @brief   配置文件
  ******************************************************************************
  * @attention
  *
  * None
  ******************************************************************************
  */

#include "config.h"
#include "scheduler.h"
#include "log.h"

/**
  * @brief  错误报告方式
  * @param  errCode: 错误码
  * @retval None
  */
void SCH_Error(ERROR_CODE errCode)
{
    logprintf(LOG_DEBUG, "错误码：%d", errCode);
}

/**
  * @brief  日志存储方式
  * @param  None
  * @retval None
  */
void LOGSave(void)
{
}
