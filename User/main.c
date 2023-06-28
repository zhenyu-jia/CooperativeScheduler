/**
  ******************************************************************************
  * @file    main.c
  * @author  JZY
  * @version V1.0.0
  * @date    2021-12-25
  * @brief   主程序
  ******************************************************************************
  * @attention
  *
  * None
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
static void System_Init(void);

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
    /* 系统初始化 */
    System_Init();
    
    /* 添加任务 */
    SCH_Add_Task(task1, 0, 100);
    SCH_Add_Task(task2, 10, 100);
    SCH_Add_Task(task3, 20, 100);
    SCH_Add_Task(task4, 30, 100);
    SCH_Add_Task(task5, 40, 10);
    
    /* 打开调度器 */
    SCH_Start();
    
    while(1)
    {
        SCH_Dispatch_Tasks();
    }
}

/**
  * @brief  系统初始化
  * @param  None
  * @retval None
  */
static void System_Init(void)
{
    SCH_Init_Systick();     // 调度器初始化
    Led[0].init();          // LED1 初始化
    Led[1].init();          // LED2 初始化
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
      logprintf(LOG_DEBUG, "Wrong parameters value: file %s on line %d\r\n", file, line);
  }
}
#endif

/************************ END OF FILE ************************/

