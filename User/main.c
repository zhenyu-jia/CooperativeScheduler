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
RCC_ClocksTypeDef get_rcc_clock;    // 获取当前时钟频率

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
    
    RCC_GetClocksFreq(&get_rcc_clock);
    logprintf(LOG_DEBUG, "SYSCLK_Frequency  = %d", get_rcc_clock.SYSCLK_Frequency);
    logprintf(LOG_DEBUG, "HCLK_Frequency    = %d", get_rcc_clock.HCLK_Frequency);
    logprintf(LOG_DEBUG, "PCLK1_Frequency   = %d", get_rcc_clock.PCLK1_Frequency);
    logprintf(LOG_DEBUG, "PCLK2_Frequency   = %d", get_rcc_clock.PCLK2_Frequency);
    
    /* 添加任务 */
    SCH_Add_Task(task1, 0, 100);
    SCH_Add_Task(task2, 10, 100);
    SCH_Add_Task(task3, 20, 100);
//    SCH_Add_Task(task4, 30, 100);
//    SCH_Add_Task(task5, 40, 10);
    
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
    // 时钟设置
    HSE_SetSysClock(RCC_HSE_ON, RCC_PLLMul_9);
//    // MCO 引脚初始化
//    MCO_GPIO_Config();
//    // MCO 引脚输出配置为 HSE
//    RCC_MCOConfig(RCC_MCO_HSE);
    
    // 调度器初始化
    SCH_Init_Systick();
    
    // LED 初始化
    Led[0].init();
    Led[1].init();
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

