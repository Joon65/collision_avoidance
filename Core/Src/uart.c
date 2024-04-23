/*
 * uart.c
 *
 *  Created on: Apr 18, 2024
 *      Author: joond
 */
#include <stdarg.h>
#include "main.h"

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size)
{
	if(huart->Instance == UART4)
	{
		sen0337_buf_size = size;
		sensor_data_ready = 1;
		HAL_UARTEx_ReceiveToIdle_DMA(&huart4, sen0337_sensor, DMA_BUF_SIZE);
		__HAL_DMA_DISABLE_IT(&hdma_uart4_rx, DMA_IT_HT);
	}
}

void log_printf_dma(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	uint16_t	size;

	//HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, SET);

	vsnprintf(LogDataToPC, sizeof(LogDataToPC), format, args);

	va_end(args);


	while(1)
	{
		if(log_buf_available == 1)
			break;
	}


	log_buf_available = 0;
	size = strlen(LogDataToPC);
	HAL_UART_Transmit_DMA(&huart3, LogDataToPC, size);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART3)
	{
		log_buf_available = 1;
		//HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, RESET);
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	}
}
