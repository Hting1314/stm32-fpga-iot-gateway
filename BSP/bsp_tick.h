#ifndef __BSP_TICK_H
#define __BSP_TICK_H

#include "stm32f4xx_hal.h"
#include "stdio.h"

void TICK_Init(void);
void HAL_SYSTICK_Callback(void);
uint32_t TICK_Get10ms(void);

#endif