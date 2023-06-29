/**
  ******************************************************************************
  * @file    clkconfig.h
  * @author  JZY
  * @version V1.0.0
  * @date    2023-6-25
  * @brief   None
  ******************************************************************************
  * @attention
  *
  * None
  ******************************************************************************
  */

#ifndef __CLKCONFIG_H
#define __CLKCONFIG_H

#include "stm32f10x.h"

void HSE_SetSysClock(uint32_t RCC_HSE, uint32_t pllmul);
void HSI_SetSysClock(uint32_t pllmul);
void MCO_GPIO_Config(void);

#endif  /* __CLKCONFIG_H */
