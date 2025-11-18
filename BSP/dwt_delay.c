#include "dwt_delay.h"

void DWT_Delay_Init(void)
{
    /* 使能 DWT 外设计数器 (CYCCNT) */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

/* us 级延时：基于 SystemCoreClock 和 CYCCNT */
void DWT_Delay_us(uint32_t us)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t ticks = us * (SystemCoreClock / 1000000U); // 每 us 需要的时钟周期数

    while ((DWT->CYCCNT - start) < ticks)
    {
        /* busy wait */
    }
}