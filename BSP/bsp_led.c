#include "bsp_led.h"
#include "main.h"        //包含 PG13 和 PG14 的定义

extern  uint8_t g_led_mode;

void LED_Init(void)
{
    // 这里假设 LED 连接在 PG13/PG14 上
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOG_CLK_ENABLE();  // 使能 GPIOG 时钟

    // 配置 PG13 和 PG14 为输出模式
    GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
}

void LED_On(void)
{
	
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_RESET); 
	
}

void LED_Off(void)
{
	
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_SET);
	
}

void LED_Toggle(void)
{
	
	HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_13);
	HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_14);
	
}

void LED_Run(void)
{
    // 根据当前模式切换 LED 行为
    switch (g_led_mode) {
        case 0:
            LED_Off();  // LED 常灭
            break;
        case 1:
            LED_On();   // LED 常亮
            break;
        case 2:
            LED_Toggle();  // LED 闪烁
            HAL_Delay(500); // 延时控制闪烁频率
            break;
        default:
            LED_Off();
            break;
    }
}
