#ifndef __BSP_USART_H
#define __BSP_USART_H

#include "stm32f4xx_hal.h"
#include "stdio.h"

extern UART_HandleTypeDef huart1;

void USART_Debug_Init(void);
void USART_SendString(const char *str);

#endif
