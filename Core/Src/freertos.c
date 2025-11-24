/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp_led.h"
#include "bsp_dht11.h"
#include "bsp_tick.h"
#include "bsp_usart.h"
#include "app_task.h"
#include "bsp_key.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

LatestSensor_t g_lateseSensor = {0};
volatile DisplayMode_t g_display_mode = DISPLAY_MODE_TEMP;



//任务句柄
osThreadId_t sensorTaskHandle;
osThreadId_t uartTaskHandle;
osThreadId_t ledTaskHandle;
osThreadId_t keyTaskHandle;

//消息队列句柄
osMessageQueueId_t sensorQueueHandle;

//最新传感器数据 （给LED使用）
LatestSensor_t g_latestSensor = {0};

//队列属性 
const osMessageQueueAttr_t sensorQueue_attributes = {
	.name = "sensorQueue"
};

const osThreadAttr_t keyTask_attributes = {
	.name       ="Key_Task",
	.stack_size =256*4,
	.priority   =(osPriority_t)osPriorityNormal,
};

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
//osThreadId_t defaultTaskHandle;
//const osThreadAttr_t defaultTask_attributes = {
//  .name = "defaultTask",
//  .stack_size = 128 * 4,
//  .priority = (osPriority_t) osPriorityNormal,
//};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

//void StartDefaultTask(void *argument);
//void StartSensorTask(void *argument);
//void StartUARTTask(void *argument);
//void StartLEDTask(void *argument);
//void StartKeyTask(void *argument); 

const osThreadAttr_t sensorTask_attributes = {
	.name    = "Sensor_Task",
	.stack_size = 512*4,
	.priority = (osPriority_t)osPriorityAboveNormal,
};

const osThreadAttr_t uartTask_attributes = {
	.name    = "UART_Task",
	.stack_size = 512 * 4,
	.priority  = (osPriority_t)osPriorityNormal,
};

const osThreadAttr_t ledTask_attributes = {
	.name     = "LED_Task",
	.stack_size = 256*4,
	.priority = (osPriority)osPriorityBelowNormal,
};


//static osThreadId_t ledTaskHandle;
//static const osThreadAttr_t ledTaskAttr = {
//	.name = "LED_Task",
//	.stack_size = 512*4,
//	.priority = (osPriority_t)osPriorityNormal
//};

//static void StartLEDTask(void *argument)
//{
//	LED_Init();
//	for(;;)
//	{
//		LED_Toggle();
//		osDelay(500);
//	}
//}

/* USER CODE END FunctionPrototypes */

//void StartDefaultTask(void *argument);
//static void StartLEDTask(void *argument);
void StartSensorTask(void *argument)
{
	DHT11_Frame_t frame;
	
	SensorMsg_t msg;

    for (;;)                              
    {
        printf("[Sensor] before DHT11_Read\r\n");

        msg.status = DHT11_Read(&msg.dht);

        printf("[Sensor] after DHT11_Read, status=%d\r\n", msg.status);

        msg.tick10ms = TICK_Get10ms();

        g_latestSensor.dht    = msg.dht;
        g_latestSensor.status = msg.status;

        osMessageQueuePut(sensorQueueHandle, &msg, 0, 0);

        osDelay(2000);
    }
//	//调试用的简单逻辑
//	SensorMsg_t msg;
//    uint32_t cnt = 0;

//    for (;;)
//    {
//        msg.status = DHT11_OK;           // 先假装传感器一直 OK
//        msg.dht.temp_int = 25 + (cnt % 5);
//        msg.dht.temp_dec = 0;
//        msg.dht.humi_int = 60;
//        msg.dht.humi_dec = 0;
//        msg.tick10ms = TICK_Get10ms();

//        g_latestSensor.dht    = msg.dht;
//        g_latestSensor.status = msg.status;

//        osMessageQueuePut(sensorQueueHandle, &msg, 0, 0);

//        cnt++;
//        osDelay(2000);                   // 必须有 osDelay，保证任务让出 CPU
//    }
//	
//	SensorMsg_t msg;
//	
//	for(;;)
//	{
//		msg.status = DHT11_Read(&msg.dht);
//		msg.tick10ms = TICK_Get10ms();
//		
//		g_latestSensor.dht   = msg.dht;
//		g_latestSensor.status = msg.status;
//		
//		(void)osMessageQueuePut(sensorQueueHandle,&msg,0,0);
//		
//		osDelay(2000);
//		
//	}
}

void StartUARTTask(void*argument)
{
	SensorMsg_t msg;

    for (;;)
    {
        osStatus_t st = osMessageQueueGet(sensorQueueHandle, &msg, NULL, osWaitForever);
        if (st == osOK)
        {
					switch (msg.status)
					{
						case DHT11_OK:
								printf("[UART] status=%d, T=%d.%d, H=%d.%d, tick=%lu\r\n",
                   msg.status,
                   msg.dht.temp_int, msg.dht.temp_dec,
                   msg.dht.humi_int, msg.dht.humi_dec,
                   (unsigned long)msg.tick10ms);
								break;
						
						case DHT11_TIMEOUT:
								printf("[UART]: S=TIMEOUT; tick=%lu\r\n",
											(unsigned long)msg.tick10ms);
								break;
						
						case DHT11_CHECKSUM_ERR:
								printf("[UART]: S=CHECKSUM_ERR; tick=%lu\r\n",
												(unsigned long)msg.tick10ms);
								break;
						
						default:
								printf("[UART]: S=UNKNOWN_ERR; tick=%lu\r\n",
												(unsigned long)msg.tick10ms);
								break;
					}
				}
			}
		}
            
//	//调试用的简单逻辑
//	uint32_t cnt = 0;

//    for (;;)
//    {
//        printf("UART Task alive: %lu\r\n", (unsigned long)cnt++);
//        osDelay(1000);
//    }
//	SensorMsg_t msg;
//	 for (;;)
//    {
//        // 永久等待队列消息
//        osStatus_t status = osMessageQueueGet(sensorQueueHandle, &msg, NULL, osWaitForever);
//        if (status == osOK)
//        {
//            if (msg.status == DHT11_OK)
//            {
//                printf("T=%d.%dC; H=%d.%d%%; S=OK; tick=%lu\r\n",
//                       msg.dht.temp_int, msg.dht.temp_dec,
//                       msg.dht.humi_int, msg.dht.humi_dec,
//                       (unsigned long)msg.tick10ms);
//            }
//            else if (msg.status == DHT11_TIMEOUT)
//            {
//                printf("DHT11 TIMEOUT; tick=%lu\r\n", (unsigned long)msg.tick10ms);
//            }
//            else if (msg.status == DHT11_CHECKSUM_ERR)
//            {
//                printf("DHT11 CHECKSUM_ERR; tick=%lu\r\n", (unsigned long)msg.tick10ms);
//            }
//            else
//            {
//                printf("DHT11 UNKNOWN_ERR; tick=%lu\r\n", (unsigned long)msg.tick10ms);
//            }
//        }
//    }
//}

void StartLEDTask(void *argument)
{
    for (;;)
    {
        LatestSensor_t snap = g_latestSensor;
        DisplayMode_t mode  = g_display_mode;

        if (snap.status != DHT11_OK)
        {
            // 传感器异常：错误闪烁模式（例如双闪）
            LED_Toggle();
            osDelay(150);
            continue;
        }

        switch (mode)
        {
        case DISPLAY_MODE_TEMP:
            // 温度模式：温度越高越快闪
            if (snap.dht.temp_int >= 28)
            {
                LED_Toggle();
                osDelay(200);
            }
            else
            {
                LED_Toggle();
                osDelay(600);
            }
            break;

        case DISPLAY_MODE_HUMI:
            // 湿度模式：湿度越高越快闪（只是示例，你可以自己定义）
            if (snap.dht.humi_int >= 70)
            {
                LED_Toggle();
                osDelay(200);
            }
            else
            {
                LED_Toggle();
                osDelay(600);
            }
            break;

        case DISPLAY_MODE_ALERT:
            // 报警模式：只有温度超过阈值才闪，否则常灭
            if (snap.dht.temp_int >= 30)
            {
                LED_Toggle();
                osDelay(150);
            }
            else
            {
                LED_Off();
                osDelay(300);
            }
            break;

        default:
            LED_Off();
            osDelay(500);
            break;
        }
    }
}

//void StartLEDTask(void *argument)
//{
//			LED_Init();

//    for (;;)
//    {
//        LED_Toggle();
//        osDelay(500);   // 0.5 秒闪一次
//    }
		
void StartKeyTask(void *argument)
{
    uint32_t last_press_tick = 0;
		uint32_t cnt = 0;
	
    for (;;)
    {
			
			printf("[KEY] task alive: %lu\r\n", (unsigned long)cnt++);
			osDelay(1000);
			
			printf("[KEY_TASK] g_key_int_flag=%d\r\n", g_key_int_flag);
			
        if (g_key_int_flag)
        {
            g_key_int_flag = 0;
						printf("[KEY_TASK] Key flag detected\r\n");

            uint32_t now = TICK_Get10ms();
						printf("[KEY_TASK] now = %lu, last_press_tick = %lu\r\n", now, last_press_tick);

            if (now - last_press_tick >= 20)   // 200ms 消抖
            {
                last_press_tick = now;

                g_display_mode = (g_display_mode + 1) % DISPLAY_MODE_MAX;

                printf("[KEY] mode switched to %d\r\n", g_display_mode);
            }
        }

        osDelay(10);
    }
}


//    for (;;)
//    {
//        LatestSensor_t snap = g_latestSensor; // 拷贝一份局部快照

//        if (snap.status != DHT11_OK)
//        {
//            // 传感器有问题 → 例如常灭
//            LED_Off();
//            osDelay(300);
//        }
//        else
//        {
//            int temp = snap.dht.temp_int;  // 只看整数部分

//            if (temp >= 28)
//            {
//                LED_Toggle();
//                osDelay(200);
//            }
//            else
//            {
//                LED_Toggle();
//                osDelay(600);
//            }
//        }
//    }
//}


void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */


  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
	 sensorQueueHandle = osMessageQueueNew(8, sizeof(SensorMsg_t), &sensorQueue_attributes);
    if (sensorQueueHandle == NULL)
    {
        // 这里可以打印错误，或者进死循环
        // printf("sensorQueue create failed\r\n");
        while(1);
    }

    // 2. 创建任务：Sensor / UART / LED
    sensorTaskHandle = osThreadNew(StartSensorTask, NULL, &sensorTask_attributes);
    uartTaskHandle   = osThreadNew(StartUARTTask,   NULL, &uartTask_attributes);
    ledTaskHandle    = osThreadNew(StartLEDTask,    NULL, &ledTask_attributes);
		keyTaskHandle    = osThreadNew(StartKeyTask,    NULL, &keyTask_attributes);
		
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
//void StartLEDTask(void *argument)
//{
//  /* USER CODE BEGIN StartDefaultTask */
//	LED_Init();
//  /* Infinite loop */
//  for(;;)
//  {
//    LED_Toggle();
//		osDelay(500);
//  }
//  /* USER CODE END StartDefaultTask */
//}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
