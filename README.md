# STM32F407 FreeRTOS Multi-Task Demo

## 项目简介
本项目基于 STM32F407 + FreeRTOS，实现了多任务温湿度采集、串口打印以及 LED 显示。

## 功能模块
- **Sensor_Task**：读取 DHT11（2s 周期）
- **UART_Task**：格式化串口输出
- **LED_Task**：根据温度动态闪烁
- **TIM3 Tick**：提供 10ms tick
- **DWT 微秒延时**：用于精确时序读取 DHT11

## 任务通信
Sensor_Task ----> Queue ----> UART_Task
|
+----> LatestSensor (LED)
## 编译方式
- 使用 Keil MDK / CubeMX 生成代码
- FreeRTOS 使用 CMSIS-RTOS v2 API

## 亮点
- 任务间通信采用 RTOS 消息队列
- DHT11 时序使用 DWT 微秒级延时
- 所有超时时间均有保护，避免任务卡死

