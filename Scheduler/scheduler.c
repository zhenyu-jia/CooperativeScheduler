/**
  ******************************************************************************
  * @file    scheduler.c
  * @author  JZY
  * @version V1.0.0
  * @date    2021-12-25
  * @brief   合作式调度器
  ******************************************************************************
  * @attention
  *
  * None
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "scheduler.h"


/* 公用变量定义 --------------------------------------------------------------*/
sTask SCH_tasks_G[SCH_MAX_TASKS];   // 任务队列
ERROR_CODE Error_code_G = NO_ERROR; // 错误代码

/* 私有函数原型 --------------------------------------------------------------*/
static void SCH_Go_To_Sleep(void);
__attribute__((weak)) void SCH_Error(ERROR_CODE errCode);

/* 私有变量 ------------------------------------------------------------------*/
static tWord Error_tick_count_G;        // 跟踪自从上一次记录错误以来的时间
static ERROR_CODE Last_error_code_G;    // 上次的错误代码（在 1 分钟之后复位）

/**
  * @brief  “调度”函数
  *     当一个任务（函数）需要运行时，SCH_Dispatch_Tasks() 将运行它
  *     该函数在主循环中被调用
  * @param  None
  * @retval None
  */
void SCH_Dispatch_Tasks(void)
{
    tByte Index;
    
    /* 调度（运行）下一个任务（如果有任务就绪） */
    for(Index = 0; Index < SCH_MAX_TASKS; Index++)
    {
        if(SCH_tasks_G[Index].RunMe > 0)
        {
            (*SCH_tasks_G[Index].pTask)();  // 运行任务
            SCH_tasks_G[Index].RunMe -= 1;  // 复位/降低 RunMe 标志
            
            // 周期性的任务将自动的再次运行
            // 如果是单次任务，将它从列表中删除
            if(SCH_tasks_G[Index].Period == 0)
            {
                SCH_Delete_Task(Index);
            }
        }
    }
    
    /* 报告系统状况 */
    SCH_Report_Status();
    /* 调度器进入低功耗模式 */
    SCH_Go_To_Sleep();
}

/**
  * @brief  添加任务函数
  *     当一个任务（函数）需要运行时，SCH_Dispatch_Tasks() 将运行它
  *     该函数在主循环中被调用
  * @param  pFunction: 被调度函数的名称，注意被调度函数必须是 void(void) 类型，
  *     即没有参数，没有返回值
  * @param  DELAY: 任务第一次运行之前的间隔（单位：时标，即调度器周期）
  * @param  PERIOD: 如果为 0，该任务在 DELAY 之后的时间被调用一次，
  *                 若不为 0，那么该任务将被重复调用，周期是 PERIOD（单位：时标）
  * @retval 返回被添加任务在任务队列中的位置。
  *     如果返回值等于 SCH_MAX_TASKS，则说明任务队列空间不够，该任务无法添加到任务队列
  *     如果返回值小于 SCH_MAX_TASKS，那么该任务被成功添加。
  *     注意：
  *         如果要删除任务，则要用到返回值，参见 SCH_Delete_Task()
  *     例子：
  *     Task_ID = SCH_Add_Task(Task1, 1000, 0);
  *     使函数 Task1() 在 1000 个调度器时标之后运行 1 次
  *     Task_ID = SCH_Add_Task(Task2, 0, 1000);
  *     使函数 Task2() 每隔 1000 个调度器时标运行一次（周期运行），且第一次是在启用调度器后立即运行
  *     Task_ID = SCH_Add_Task(Task3, 300, 1000);
  *     使函数 Task3() 每隔 1000 个调度器时标运行一次（周期运行），且第一次是在启用调度器后 300 个调度器时标后运行
  */
tByte SCH_Add_Task(void (*pFunction)(), 
                   const tWord DELAY, 
                   const tWord PERIOD)
{
    tByte Index = 0;
    
    /* 首先在任务队列中找到一个空隙（如果有的话） */
    while((SCH_tasks_G[Index].pTask != 0) && (Index < SCH_MAX_TASKS))
    {
        Index++;
    }
    
    /* 是否已经到达队列的结尾？ */
    if(Index == SCH_MAX_TASKS)
    {
        // 任务队列已满
        // 设置全局错误变量
        Error_code_G = ERROR_SCH_TOO_MANY_TASKS;
        // 同时返回错误代码
        return SCH_MAX_TASKS;
    }
    
    /* 运行到这里，说明任务队列中有空间 */
    SCH_tasks_G[Index].pTask    = pFunction;
    SCH_tasks_G[Index].Delay    = DELAY;
    SCH_tasks_G[Index].Period   = PERIOD;
    SCH_tasks_G[Index].RunMe    = 0;
    
    return Index;   // 返回任务的位置（以便以后删除）
}

/**
  * @brief  删除任务函数
  *     注意：并不从存储器中删除相关的函数，仅仅是不再由调度器调用这个任务
  * @param  TASK_INDEX: 任务索引，由 SCH_Add_Task()提供
  * @retval RETURN_ERROR 或 RETURN_NORMAL
  */
RETURN_CODE SCH_Delete_Task(const tByte TASK_INDEX)
{
    RETURN_CODE Return_code;
    
    if(SCH_tasks_G[TASK_INDEX].pTask == 0)
    {
        // 没有任务
        // 设置全局错误变量
        Error_code_G = ERROR_SCH_CANNOT_DELETE_TASK;
        // 同时返回错误代码
        Return_code = RETURN_ERROR;
    }
    
    else
    {
        Return_code = RETURN_NORMAL;
    }
    
    SCH_tasks_G[TASK_INDEX].pTask   = 0x0000;
    SCH_tasks_G[TASK_INDEX].Delay   = 0;
    SCH_tasks_G[TASK_INDEX].Period  = 0;
    SCH_tasks_G[TASK_INDEX].RunMe   = 0;
    
    return Return_code;     // 返回状态
}

/**
  * @brief  用来显示错误代码的简单函数
  *     可以用 LED、蜂鸣器、或者串口打印等
  *     本版本利用 printf() 打印错误报告
  * @param  None
  * @retval None
  */
void SCH_Report_Status(void)
{
#ifdef SCH_REPORT_ERRORS
    if(Error_code_G != Last_error_code_G)
    {
        /* 错误报告方式 */
        SCH_Error(Error_code_G);
        
        /*-----------------------------------*/
        
        Last_error_code_G = Error_code_G;
        
        if(Error_code_G != NO_ERROR)
        {
            Error_tick_count_G = SCH_REPORT_ERRORS_TICKS;
        }
        
        else
        {
            Error_tick_count_G = 0;
        }
    }
    
    else
    {
        if(Error_tick_count_G != 0)
        {
            if(--Error_tick_count_G == 0)
            {
                Error_code_G = NO_ERROR;    // 复位错误代码
            }
        }
    }
#endif  /* SCH_REPORT_ERRORS */
}

/**
  * @brief  CPU 进入低功耗模式
  *     调度器在时标之间进入“低功耗模式”以节省功耗，下一个时钟时标将使处理器返回到正常操作状态
  *     注意：
  *     如果这个函数由宏来实现，或简单地将这里的代码粘贴到“调度”函数中，那么可以带来少量的性能改善
  *     然而，通过函数调用的方式来实现，可以在开发期间更容易地使用 keil 硬件模拟器中的“性能分析器”
  *     来估计调度器的性能。
  *     ***如果使用看门狗的话，可能需要禁止这个功能***
  *     ***根据硬件的需要修改***
  * @param  None
  * @retval None
  */
static void SCH_Go_To_Sleep(void)
{
    /* 进入低功耗模式 */ 
    
}

/**
  * @brief  调度器更新函数
  *     这个函数在 Systick 定时器中断服务函数中调用
  * @param  None
  * @retval None
  */
inline void SCH_Updata(void)
{
    tByte Index;
    
    for(Index = 0; Index < SCH_MAX_TASKS; Index++)
    {
        // 检查这里是否有任务
        if(SCH_tasks_G[Index].pTask)
        {
            if(SCH_tasks_G[Index].Delay == 0)
            {
                SCH_tasks_G[Index].RunMe += 1;  // "RunMe" 标志加 1
                
                if(SCH_tasks_G[Index].Period)
                {
                    // 调度定期的任务再次运行
                    SCH_tasks_G[Index].Delay = SCH_tasks_G[Index].Period;
                }
            }
            
            else
            {
                // 任务还未准备好，延时减 1
                SCH_tasks_G[Index].Delay -= 1;
            }
        }
    }
}

/**
  * @brief  调度器定时器 Systick 初始化函数
  * @param  None
  * @retval None
  */
void SCH_Init_Systick(void)
{
    tByte i;
    
    for(i = 0; i < SCH_MAX_TASKS; i++)
    {
        SCH_Delete_Task(i);
    }
    
    // 复位全局错误变量
    // SCH_Delete_Task() 将产生一个错误代码
    // 因为任务队列是空的
    Error_code_G = NO_ERROR;
    
    if(SysTick_Config(SCH_SYSTICK_TICKS))
    { 
        /* Capture error */ 
        while (1);
    }
    
    SysTick->CTRL &= ~(1 << 2);     // 选择时钟源为外部时钟源
    SysTick->CTRL &= ~(1 << 1);     // 关闭 SysTick 中断 
}

/**
  * @brief  通过允许中断来启动调度器
  * @param  None
  * @retval None
  */
void SCH_Start(void)
{
    SysTick->VAL = 0;
    SysTick->CTRL |= (1 << 1);     // 打开 SysTick 中断 
}

/**
  * @brief  错误报告方式
  * @param  errCode: 错误码
  * @retval None
  */
__attribute__((weak)) void SCH_Error(ERROR_CODE errCode)
{
}
