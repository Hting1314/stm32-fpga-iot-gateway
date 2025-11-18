#ifndef __BSP_DHT11_H
#define __BSP_DHT11_H

#include "stm32f4xx_hal.h"

typedef struct
{
	uint8_t humi_int;
	uint8_t humi_dec;
	uint8_t temp_int;
	uint8_t temp_dec;
}DHT11_Data_t;

typedef enum
{
	DHT11_OK = 0,
	DHT11_TIMEOUT,
	DHT11_CHECKSUM_ERR
}DHT11_Status_t;

void DHT11_Init(void);
DHT11_Status_t DHT11_Read(DHT11_Data_t *data);

#endif
