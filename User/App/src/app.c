/**
  ******************************************************************************
  * @file    app.c
  * @author  JZY
  * @version V1.0.0
  * @date    2021-12-25
  * @brief   应用程序
  ******************************************************************************
  * @attention
  *
  * None
  ******************************************************************************
  */

#include "app.h"
#include "led.h"
#include "log.h"


void task1(void)
{
    Led[0].toggle();
}

void task2(void)
{
    Led[1].toggle();
}

void task3(void)
{
    
}

void task4(void)
{
    logprintf(LOG_DEBUG, "task4 runing.");
}

void task5(void)
{
    logprintf(LOG_WARN, "task5 runing.");
}
