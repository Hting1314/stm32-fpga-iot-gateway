#include "bsp_tick.h"
#include "stm32f4xx_hal.h"

extern TIM_HandleTypeDef htim3;            // 定时器句柄
volatile uint32_t g_tick10ms = 0;        //定时器每10ms 增加的计时数
//extern volatile uint32_t g_key_debounce_timer;

void TICK_Init(void)
{
	
	HAL_TIM_Base_Start_IT(&htim3);			//启动 TIM3 定时器 + 打开更新中断

}


uint32_t TICK_Get10ms(void)            //封装为 TICK_Get10ms——供外部调用
{
	
	return HAL_GetTick()/10;         
	
}

//void HAL_SYSTICK_Callback(void)
//{
//	if(g_key_debounce_timer > 0)
//	{
//		g_key_debounce_timer--;
//	}
//}

/*   定时器中断回调函数： 每次TIM3 更新时， 计数加1   */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	 if (htim->Instance == TIM6)
    {
        HAL_IncTick();
//        printf("[TIM6] Tick Incremented\r\n");  // 用来调试
    }
//    else if (htim->Instance == TIM3)
//    {
//        g_tick10ms++;
//        printf("[TIM3] g_tick10ms = %lu\r\n", g_tick10ms);  // 用来调试
//    }
}
