/**
  ******************************************************************************
  * @file    led.h
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

#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"


#define LED1_GPIO_PORT  GPIOB
#define LED1_GPIO_CLK   RCC_APB2Periph_GPIOB
#define LED1_GPIO_PIN   GPIO_Pin_5

#define LED2_GPIO_PORT  GPIOE
#define LED2_GPIO_CLK   RCC_APB2Periph_GPIOE
#define LED2_GPIO_PIN   GPIO_Pin_5

typedef enum
{
    LOW = 0,
    HIGH = !LOW,
}LED_Open_Level;

typedef enum
{
    CLOSE = 0,
    OPEN = !LOW,
}LED_State;

typedef struct
{
    GPIO_TypeDef* port;
    uint16_t pin;
    uint32_t clk;                   // 时钟
    LED_Open_Level active_level;    // 打开时的电平电平
    LED_State default_state;        // 默认状态
    LED_State pin_state;            // 记录LED状态
}LED_GPIO_Data;

typedef struct
{
    LED_GPIO_Data gpio_data;
    void (*init)(void);
    void (*open)(void);
    void (*close)(void);
    void (*toggle)(void);
}LED;

extern LED Led[2];

#endif  /* __LED_H */
