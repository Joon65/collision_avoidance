/*
 * sen0337_sensor.c
 *
 *  Created on: Apr 18, 2024
 *      Author: joond
 */

#include "main.h"
#include "sen0337_sensor.h"

uint16_t process_sen0337_sensor_data(uint8_t *buf)
{
	sen0337	sen0337_data;
	uint32_t	distance;

	memcpy(&sen0337_data, sen0337_sensor, sen0337_buf_size);
	HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);

	distance = (sen0337_data.distance[2] << 16) | (sen0337_data.distance[1] << 8) | sen0337_data.distance[0];
	log_printf_dma("%d\r\n", distance);
	return OK;
}
