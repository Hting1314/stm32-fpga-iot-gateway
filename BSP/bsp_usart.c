#include "bsp_usart.h"
#include "usart.h"
#include "string.h"

void USART_Debug_Init(void)            
{
	
	MX_USART1_UART_Init();
	
}

void USART_SendString(const char *str)
{
	
	HAL_UART_Transmit(&huart1, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
	
}

int fputc(int ch, FILE *f)
{
	
	HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1,HAL_MAX_DELAY);    	//HAL_UART_Transmit 需要一个unit8_t类型指针
	return ch;
	
}