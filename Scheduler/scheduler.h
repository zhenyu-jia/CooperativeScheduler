/**
  ******************************************************************************
  * @file    scheduler.h
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

#ifndef __SCHEDULER_H
#define __SCHEDULER_H

#include "stm32f10x.h"

/* 调度器变量类型重定义 */
#ifndef tWord
#define tWord   uint16_t
#endif  /* tWord */

#ifndef tByte
#define tByte   uint8_t
#endif  /* tByte */

/* 公用的常数 ---------------------------------------------------------------*/
/* 任务最大数量（每个新建项目都必须修改） */
#define SCH_MAX_TASKS                   (10U)

/* Systick 定时器参数 */
#define SCH_SYSTICK_TIME_CYCLE_PERIOD   (1U)  // Systick 时间周期（单位：ms）
#define SCH_SYSTICK_TICKS               (SCH_SYSTICK_TIME_CYCLE_PERIOD * SystemCoreClock / 1000 / 8)

/* 如果不需要错误报告，将该行注释掉 */
#define SCH_REPORT_ERRORS
/* 错误代码报告周期（单位：ms），当时标周期为 1ms 时，每隔 1 分钟报告 */
#define SCH_REPORT_ERRORS_CYCLE_PERIOD  (60000U)
#define SCH_REPORT_ERRORS_TICKS         (SCH_REPORT_ERRORS_CYCLE_PERIOD * SCH_SYSTICK_TIME_CYCLE_PERIOD)

/* 公用的数据类型 -----------------------------------------------------------*/
/*
 * 任务数据类型
 * 每个任务的存储器的总和是7个字节
 */
typedef struct
{
    void (*pTask)(void);    // 指向任务的指针（必须是一个 "void(void)" 函数）
    tWord Delay;            // 延迟（时标）
    tWord Period;           // 在连续的运行之间的间隔（时标）
    tByte RunMe;            // 当任务需要运行时（由调度器）加 1
}sTask;

/*
 * 返回代码
 */
typedef enum
{
    RETURN_NORMAL = 0,
    RETURN_ERROR  = 1
}RETURN_CODE;

/*
 * 错误代码
 */
typedef enum
{
    NO_ERROR = 0,                   // 没有错误
    ERROR_SCH_CANNOT_DELETE_TASK,   // 任务队列已空
    ERROR_SCH_TOO_MANY_TASKS        // 任务队列已满
}ERROR_CODE;

/* 公用的函数原型 -----------------------------------------------------------*/
/*
 * 调度器内核函数
 */
void SCH_Dispatch_Tasks(void);
tByte SCH_Add_Task(void (*pFunction)(), const tWord DELAY, const tWord PERIOD);
RETURN_CODE SCH_Delete_Task(const tByte TASK_INDEX);
void SCH_Report_Status(void);
void SCH_Updata(void);
void SCH_Init_Systick(void);
void SCH_Start(void);

#endif  /* __SCHEDULER_H */
