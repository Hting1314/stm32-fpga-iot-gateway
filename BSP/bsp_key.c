#include "bsp_key.h"
//#include "bsp_tick.h"
//#include "bsp_led.h"
//#include "bsp_usart.h"   
#include <stdio.h>
#include "stm32f4xx_hal.h"

//extern volatile uint32_t g_tick10ms;       //引入定时器提供的 g_tick10ms
uint8_t g_led_mode = 0;

//volatile uint8_t g_key_state = 0;					
//volatile uint8_t g_key_stable = 0;
//volatile uint32_t g_key_debounce_timer = 0;     //按键消抖计时器 ，单位：10ms
//volatile uint8_t g_key_released = 1;        // 按键松开标志，用于避免重复切换模式

static volatile uint32_t key_last_ms = 0;
static volatile uint32_t key_event = 0;

void KEY_Init(void)
{
//	g_key_state = 0;
//	g_key_stable = 0;
//	g_key_debounce_timer = 0;
	key_event = 0;
	key_last_ms = 0;
	
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_6)
    {
        uint32_t now = HAL_GetTick();
        // 200ms 简单消抖：防止一按多触发
        if (now - key_last_ms > 200U)
        {
            key_event  = 1;
            key_last_ms = now;
        }
    }
}

/* 在主循环里调用：把一次按键事件转换为模式切换（可打印） */
void KEY_ProcessEvent(void)
{
    if (key_event)
    {
        key_event = 0;
        g_led_mode = (g_led_mode + 1) % 3;  // 0:灭 1:亮 2:闪
        printf("[KEY] pressed -> g_led_mode=%d\r\n", g_led_mode);
    }
}

//void KEY_Debounce_Check(void)
//{
//	if (g_key_debounce_timer == 0) 
//    {
//        if (HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_6) == GPIO_PIN_RESET) // 按键按下，假设是低电平触发
//        {
//            g_key_state = 1;  // 按键按下
//            g_key_stable = 1; // 按键稳定
//        }
//        else 
//        {
//            g_key_state = 0;  // 按键松开
//            g_key_stable = 1; // 按键稳定
//        }

//        // 重置消抖计时器，等待下一个检查周期
//        g_key_debounce_timer = 10;  // 重置消抖计时器
//    }
//}

// 返回按键稳定状态
//uint8_t KEY_IsPressed(void)
//{
//    return g_key_stable;  
//}

//void KEY_ModeSwitch(void)
//{
//    // 只有在按键按下且稳定，且上一次是“松开状态允许触发”时，才切换模式
//    if (g_key_state == 1 && g_key_stable == 1 && g_key_released == 1)
//    {
//        // 模式依次：0 -> 1 -> 2 -> 0 ...
//        g_led_mode = (g_led_mode + 1) % 3;

//        printf("[KEY] pressed, g_led_mode = %d\r\n", g_led_mode);

//        g_key_released = 0;   // 本次按键已处理，等待松开
//    }

//    // 检测按键已松开，准备下一次触发
//    if (g_key_state == 0 && g_key_stable == 1)
//    {
//        g_key_released = 1;
//    }
//}

// 检查按键是否松开，防止连续切换
//void KEY_ModeSwitch(void)
//{
//	// 只有在按键松开后才切换模式
//    if (g_key_state == 1 && g_key_stable == 1 && g_key_released == 1) 
//    {
//        // 切换 LED 模式
//        switch (g_led_mode)
//        {
//            case 0:
//                g_led_mode = 1;  // 切换到 LED 常亮模式
//                break;
//            case 1:
//                g_led_mode = 2;  // 切换到 LED 常灭模式
//                break;
//            case 2:
//                g_led_mode = 3;  // 切换到 LED 闪烁模式
//                break;
//            default:
//                g_led_mode = 0;  // 切换到 LED 闪烁模式
//                break;
//        }

//        // 按键切换完成，标记按键已松开
//        g_key_released = 0;
//    }
//    // 如果按键松开，允许再次切换模式
//    if (g_key_state == 0 && g_key_stable == 1)
//    {
//        g_key_released = 1;
//    }
//}