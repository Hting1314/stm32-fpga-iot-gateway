#include "bsp_dht11.h"
#include "dwt_delay.h"

#define DHT11_PORT     GPIOB
#define DHT11_PIN      GPIO_PIN_11

/* 内部工具函数：切换引脚模式 */
static void DHT11_Pin_Output(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = DHT11_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);
}

static void DHT11_Pin_Input(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = DHT11_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);
}

/* 等待引脚进入指定状态，超时单位：us，超时返回1，成功返回0 */
static uint8_t DHT11_WaitForState(GPIO_PinState state, uint32_t timeout_us)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t ticks = timeout_us * (SystemCoreClock / 1000000U);

    while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) != state)
    {
        if ((DWT->CYCCNT - start) > ticks)
        {
            return 1; // TIMEOUT
        }
    }
    return 0; // OK
}

void DHT11_Init(void)
{
    /* 默认拉高，空闲状态 */
    DHT11_Pin_Output();
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);
}

/* 核心读取函数：严格按 DHT11 协议时序 */
DHT11_Status_t DHT11_Read(DHT11_Data_t *data)
{
    uint8_t raw[5] = {0};
    uint8_t i, j;

    /* 1. 发送起始信号：拉低 ≥18ms */
    DHT11_Pin_Output();
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_RESET);
    HAL_Delay(18);

    /* 2. 拉高 20~40us */
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);
    DWT_Delay_us(30);

    /* 3. 切换为输入，等待 DHT11 响应 */
    DHT11_Pin_Input();

    /* DHT11 响应：80us 低电平 + 80us 高电平 */

    if (DHT11_WaitForState(GPIO_PIN_RESET, 100))   // 等待变为低电平
        return DHT11_TIMEOUT;

    if (DHT11_WaitForState(GPIO_PIN_SET, 100))     // 等待变为高电平
        return DHT11_TIMEOUT;

    if (DHT11_WaitForState(GPIO_PIN_RESET, 100))   // 再次拉低，准备发送数据
        return DHT11_TIMEOUT;

    /* 4. 开始接收 40 bit 数据：每位结构 = 50us 低 + (26~28us=0 或 ~70us=1 的高) */

    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 8; j++)
        {
            /* 等待 50us 低电平结束，进入高电平 */
            if (DHT11_WaitForState(GPIO_PIN_SET, 70))
                return DHT11_TIMEOUT;

            /* 高电平开始，延时约 40us 到中间采样点 */
            DWT_Delay_us(40);

            if (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_SET)
            {
                /* 若此时仍为高电平，认为是逻辑1 */
                raw[i] |= (1 << (7 - j));

                /* 等待该位的高电平结束，回到低电平，准备下一位 */
                if (DHT11_WaitForState(GPIO_PIN_RESET, 70))
                    return DHT11_TIMEOUT;
            }
            else
            {
                /* 若此时已是低，认为是逻辑0，直接进入下一位 */
            }
        }
    }

    /* 5. 校验和 */
    uint8_t sum = raw[0] + raw[1] + raw[2] + raw[3];
    if (sum != raw[4])
    {
        return DHT11_CHECKSUM_ERR;
    }

    /* 6. 填充数据 */
    data->humi_int = raw[0];
    data->humi_dec = raw[1];
    data->temp_int = raw[2];
    data->temp_dec = raw[3];

    return DHT11_OK;
}