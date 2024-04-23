/*
 * timer.c
 *
 *  Created on: Apr 17, 2024
 *      Author: joond
 */

#include "main.h"
#include "stm32h7xx_hal.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM6)
	{
		// timer = 1ms
		HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
		one_sec_counter += 1;
		if (one_sec_counter >= 1000)
		{
			one_sec_flag = 1;
			one_sec_counter = 0;
		}
	}
}
