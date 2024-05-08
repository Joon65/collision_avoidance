/*
 * i2c.h
 *
 *  Created on: Apr 24, 2024
 *      Author: joond
 */

#ifndef INC_I2C_H_
#define INC_I2C_H_

#include "VL53L1X_api.h"

#define		I2C_ADDRESS		0x55<<1		// for test
#define		VL53L1X_I2C_ADDRESS		0x29<<1
#define		READ_ADDRESS	0x10
#define		WRITE_ADDRESS	0x20

#define		VL53L1X_CALIBRATED


enum VL53L1X_state
{
	NOT_BOOTED,
	BOOTED,
	READY,
	CALIBRATING,
	CALIBRATED,
	INITIALIZED,
};

extern uint8_t	read_I2C_byte(uint16_t address);
extern HAL_StatusTypeDef	write_I2C_byte(uint16_t address, uint8_t *data);
extern HAL_StatusTypeDef	read_I2C_multi(uint16_t device_address, uint16_t register_address, uint8_t *buf, uint16_t size);
extern VL53L1X_ERROR VL53L1X_read_measurement(uint16_t address, uint16_t *distance);

#endif /* INC_I2C_H_ */
