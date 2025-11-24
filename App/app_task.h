#ifndef __APP_TASKS_H
#define __APP_TASKS_H

#include <stdint.h>
#include "bsp_dht11.h"

//传给 UART 的消息结构
typedef struct
{
	DHT11_Frame_t dht;     //温湿度
	DHT11_Status_t status;   // OK / TIMEOUT / CHECKSUM_ERR
	uint32_t       tick10ms;    //发生时间
}SensorMsg_t;

//给 LED / 其他模块看的“最新数据”
typedef struct
{
	DHT11_Frame_t   dht;
	DHT11_Status_t status;
} LatestSensor_t;

/*   显示模式    */
typedef enum
{
	
	DISPLAY_MODE_TEMP = 0,    // 温度模式
	DISPLAY_MODE_HUMI,				// 湿度模式
	DISPLAY_MODE_ALERT,				// 报警模式
	DISPLAY_MODE_MAX
} DisplayMode_t;

extern LatestSensor_t g_latestSensor;      // 在app_freertos.c 里定义
extern volatile DisplayMode_t g_display_mode;

#endif