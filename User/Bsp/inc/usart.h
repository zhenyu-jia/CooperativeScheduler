/**
  ******************************************************************************
  * @file    usart.h
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

#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"

typedef struct
{
    void (*init)(void);
    void (*send_str)();
    void (*receive_str)();
    void (*send_hex)();
    void (*receive_hex)();
    void (*open)();
    void (*close)();
}USART;

#endif  /* __USART_H */
