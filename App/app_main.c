#include "app_main.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_dht11.h"
#include "stdio.h"


static DHT11_Frame_t dht_data;
//extern uint8_t g_led_mode;

void App_Init(void)
{
	
	LED_Init();
	KEY_Init();
	DHT11_Init();
	USART_Debug_Init();
	
	printf("App Init OK\r\n");
}

//void App_Loop(void)
//{
//	static uint32_t last_dht_ms = 0;
//	uint32_t now = HAL_GetTick();
//	
//	//每1秒读一次 DHT11，避免连续查询
//	 if(now - last_dht_ms >= 1000U)
//	 {
//		 last_dht_ms = now;
//		 
//		 DHT11_Status_t ret = DHT11_Read(&dht_data);
//		 printf("DHT11 ret = %d\r\n", ret);
//		 
//		 if(ret == DHT11_OK)
//		 {
//			printf("温度：%d.%d℃， 湿度：%d.%d％\r\n",
//							dht_data.temp_int, dht_data.temp_dec,
//							dht_data.humi_int, dht_data.humi_dec);
//		 }
//	 }
//	 
//	 KEY_ProcessEvent();

//	 LED_Run();
//	 
// }

/*		DHT11 debug部分		*/
//	DHT11_Status_t ret = DHT11_Read(&dht_data);
//	printf("DHT11 ret = %d\r\n", ret);
//	if (ret == DHT11_OK) 
//		{
//			printf("温度：%d.%d℃， 湿度：%d.%d％\r\n",
//            dht_data.temp_int, dht_data.temp_dec,
//            dht_data.humi_int, dht_data.humi_dec);
//		}
//
//		if (DHT11_Read(&dht_data) == DHT11_OK)
//	{
//		printf("温度：%d.%d℃， 湿度：%d.%d％\r\n",dht_data.temp_int,dht_data.temp_dec,dht_data.humi_int,dht_data.humi_dec);
//	}
	
	// 每次主循环时检查按键状态并切换 LED 模式
//    KEY_Debounce_Check();  // 检查按键消抖
//    KEY_ModeSwitch();      // 切换 LED 模式

//    // 根据当前模式控制 LED
//    LED_Run();
//	
//	
//}

