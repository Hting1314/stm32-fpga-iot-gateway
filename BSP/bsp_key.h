#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "stm32f4xx_hal.h"

void KEY_Init(void);          //按键初始化
uint8_t KEY_IsPressed(void);   //获取按键状态
//void KEY_Debounce_Check(void); //按键消抖检查
//void KEY_ModeSwitch(void);
void KEY_ProcessEvent(void);

#endif