#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "stm32f4xx_hal.h"

#define KEY0_GPIO_PORT  GPIOF
#define KEY0_PIN         GPIO_PIN_6

void KEY_Init(void);          //按键初始化
void KEY_EXTI_Callback(uint16_t GPIO_Pin);

/*  提供一个按键中断标志给任务用  */
extern volatile uint8_t g_key_int_flag;

//uint8_t KEY_IsPressed(void);   //获取按键状态
////void KEY_Debounce_Check(void); //按键消抖检查
////void KEY_ModeSwitch(void);
//void KEY_ProcessEvent(void);

#endif