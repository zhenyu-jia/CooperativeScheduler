/**
  ******************************************************************************
  * @file    led.c
  * @author  JZY
  * @version V1.0.0
  * @date    2021-12-25
  * @brief   led 驱动
  ******************************************************************************
  * @attention
  *
  * None
  ******************************************************************************
  */

#include "led.h"


static void LED_Open(LED_GPIO_Data* gpio_data);
static void LED_Close(LED_GPIO_Data* gpio_data);
static void LED_Init(LED_GPIO_Data* gpio_data);
static void LED_Toggle(LED_GPIO_Data* gpio_data);
void LED1_Init(void);
void LED1_Open(void);
void LED1_Close(void);
void LED1_Toggle(void);
void LED2_Init(void);
void LED2_Open(void);
void LED2_Close(void);
void LED2_Toggle(void);

LED Led[2] = {
    {
        .gpio_data.port = LED1_GPIO_PORT,
        .gpio_data.pin = LED1_GPIO_PIN,
        .gpio_data.clk = LED1_GPIO_CLK,
        .gpio_data.active_level = LOW,
        .gpio_data.default_state = CLOSE,
        .gpio_data.pin_state = CLOSE,
        .init = LED1_Init,
        .open = LED1_Open,
        .close = LED1_Close,
        .toggle = LED1_Toggle
    },
    {
        .gpio_data.port = LED2_GPIO_PORT,
        .gpio_data.pin = LED2_GPIO_PIN,
        .gpio_data.clk = LED2_GPIO_CLK,
        .gpio_data.active_level = LOW,
        .gpio_data.default_state = OPEN,
        .gpio_data.pin_state = OPEN,
        .init = LED2_Init,
        .open = LED2_Open,
        .close = LED2_Close,
        .toggle = LED2_Toggle
    }
};

void LED1_Init(void)
{
    LED_Init(&(Led[0].gpio_data));
}

void LED1_Open(void)
{
    LED_Open(&(Led[0].gpio_data));
}

void LED1_Close(void)
{
    LED_Close(&(Led[0].gpio_data));
}

void LED1_Toggle(void)
{
    LED_Toggle(&(Led[0].gpio_data));
}

void LED2_Init(void)
{
    LED_Init(&(Led[1].gpio_data));
}

void LED2_Open(void)
{
    LED_Open(&(Led[1].gpio_data));
}

void LED2_Close(void)
{
    LED_Close(&(Led[1].gpio_data));
}

void LED2_Toggle(void)
{
    LED_Toggle(&(Led[1].gpio_data));
}

static void LED_Init(LED_GPIO_Data* gpio_data)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 时钟使能
    RCC_APB2PeriphClockCmd(gpio_data->clk, ENABLE);
    
    // LED GPIO配置
    GPIO_InitStructure.GPIO_Pin     = gpio_data->pin;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_Init(gpio_data->port, &GPIO_InitStructure);
    
    // 默认状态
    gpio_data->default_state == CLOSE ? LED_Close(gpio_data) : LED_Open(gpio_data);
    
    gpio_data->pin_state = gpio_data->default_state;
}

static void LED_Open(LED_GPIO_Data* gpio_data)
{
    gpio_data->active_level == LOW ? \
        (gpio_data->port->BRR = gpio_data->pin) : \
        (gpio_data->port->BSRR = gpio_data->pin);
    
    gpio_data->pin_state = OPEN;
}

static void LED_Close(LED_GPIO_Data* gpio_data)
{
    gpio_data->active_level == LOW ? \
        (gpio_data->port->BSRR = gpio_data->pin) : \
        (gpio_data->port->BRR = gpio_data->pin);
    
    gpio_data->pin_state = CLOSE;
}

static void LED_Toggle(LED_GPIO_Data* gpio_data)
{
    gpio_data->port->ODR^=gpio_data->pin;
}
