/*
 * i2c.c
 *
 *  Created on: Apr 24, 2024
 *      Author: joond
 */

#include "main.h"
#include "i2c.h"
#include "VL53L1X_api.h"


void VL53L1X_distance_measurement(enum VL53L1X_state *state)
{
	uint16_t		distance;
	VL53L1X_ERROR	result;
	uint8_t		boot_state;
	uint8_t		state_value;
	VL53L1X_Result_t  GetResult_value;

	switch(*state)
	{
	case NOT_BOOTED:
		VL53L1X_BootState(VL53L1X_I2C_ADDRESS, &boot_state);
		if ((boot_state & 0x01) == 0)
		{
			log_printf_dma("VL53L1X is not booted yet\r\n");
		}
		else
		{
			log_printf_dma("VL53L1X is booted\r\n");

#ifdef	VL53L1X_CALIBRATED:
			*state = CALIBRATING;
#else
			*state = CALIBRATING;
#endif
		}
		break;
	case BOOTED:
		result = VL53L1X_StartRanging(VL53L1X_I2C_ADDRESS);
		log_printf_dma("VL53L1X_StartRanging : %d\r\n", result);
		if (result == 0)
		{
			*state = READY;
			log_printf_dma("VL53L1X_StartRanging : %d\r\n", result);
		}
		else
		{
			log_printf_dma("VL53L1X_StartRanging failed: %d\r\n", result);
		}
		break;

	case READY:
		//result = VL53L1X_read_measurement(VL53L1X_I2C_ADDRESS, &distance);
		//log_printf_dma("Distance : %d, %d\r\n", distance, result);
		//result = VL53L1X_GetRangeStatus(VL53L1X_I2C_ADDRESS, &state_value);
		//log_printf_dma("RangeStatus = %d, %d\r\n", state_value, result);

		result = VL53L1X_GetResult(VL53L1X_I2C_ADDRESS, &GetResult_value);
		log_printf_dma("Status = %04x, distance = %d\r\n", GetResult_value.Status, GetResult_value.Distance);

		break;

	case CALIBRATING:
		result = VL53L1X_SensorInit(VL53L1X_I2C_ADDRESS);
		log_printf_dma("Sensor Init : %d\r\n", result);

		/*
		HAL_Delay(500);

		result = VL53L1X_CalibrateOffset(VL53L1X_I2C_ADDRESS, 100, &distance);
		log_printf_dma("CalibrateOffset : %d, %d(mm)\r\n", result, distance);

		HAL_Delay(500);
		VL53L1X_CalibrateXtalk(VL53L1X_I2C_ADDRESS, 100, &distance);
		log_printf_dma("CalibrateCrossTalk : %d, %d(mm)\r\n", result, distance);
		*/

		*state = BOOTED;

		break;

	case CALIBRATED:
		break;

	default:
		log_printf_dma("VL53L1X state : %d\r\n", *state);
		break;
	}
}


uint8_t	read_I2C_byte(uint16_t address)
{
	uint8_t		data[16];
	HAL_StatusTypeDef	result;
	result = HAL_I2C_Master_Receive(&hi2c1, address, data, 1, 0xFFFFFFFF);
	if (result == HAL_OK)
	{
		return data[0];
	}
	else
		return result;
}

HAL_StatusTypeDef	write_I2C_byte(uint16_t address, uint8_t *data)
{
	HAL_StatusTypeDef	result;

	result = HAL_I2C_Master_Transmit(&hi2c1, address, data, 1, 0x10000);
	return result;
}

HAL_StatusTypeDef	read_I2C_multi(uint16_t device_address, uint16_t register_address, uint8_t *buf, uint16_t size)
{
	HAL_StatusTypeDef	result;

	result = HAL_I2C_Mem_Read(&hi2c1, device_address, register_address, 2, buf, size, 0xFFFFFFFF);
	return result;
}

HAL_StatusTypeDef	write_I2C_multi(uint16_t device_address, uint16_t register_address, uint8_t *buf, uint16_t size)
{
	HAL_StatusTypeDef	result;

	result = HAL_I2C_Mem_Write(&hi2c1, device_address, register_address, 2, buf, size, 0xFFFFFFFF);
	return result;
}


VL53L1X_ERROR	VL53L1X_start_measurement(uint16_t address)
{
	VL53L1X_ERROR  result;
	result = VL53L1X_StartRanging(address);
	return result;
}

VL53L1X_ERROR VL53L1X_stop_measurement(uint16_t address)
{
	VL53L1X_ERROR	result;
	result = VL53L1X_StopRanging(address);
	return result;
}

VL53L1X_ERROR VL53L1X_read_measurement(uint16_t address, uint16_t *distance)
{
	VL53L1X_ERROR	result;
	uint8_t			isDataReady;

	result = VL53L1X_CheckForDataReady(address, &isDataReady);
	if (isDataReady == 0)
	{
		return 1;
	}
	else if (result != 0)
	{
		return 2;
	}

	result = VL53L1X_GetDistance(address, distance);
	if (result != 0)
	{
		return 3;
	}

	result = VL53L1X_ClearInterrupt(address);
	if (result != 0)
	{
		return 4;
	}
}

