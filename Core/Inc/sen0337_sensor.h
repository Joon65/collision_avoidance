/*
 * sen0337_sensor.h
 *
 *  Created on: Apr 18, 2024
 *      Author: joond
 */

#ifndef INC_SEN0337_SENSOR_H_
#define INC_SEN0337_SENSOR_H_

typedef struct
{
	uint8_t		header;
	uint8_t		mark;			// Function mark
	uint8_t		reserved_1;
	uint8_t		id;
	uint32_t	system_time;
	uint8_t		distance[3];
	uint8_t		status;
	uint16_t	strength;
	uint8_t		reserved_2;
	uint8_t		checksum;
}sen0337;


enum sen0337_status
{
	valid,
	over_diviation,
	low_strength,
	phase_out_bounds = 4,
	HW_error,
	mismatch_phase = 7,
	underflow_fault,
	invalid_measurement = 14
};

extern uint16_t process_sen0337_sensor_data(uint8_t *buf);

#endif /* INC_SEN0337_SENSOR_H_ */
