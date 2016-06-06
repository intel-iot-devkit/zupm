/*
 * upm_servo.h
 *
 *  Created on: May 4, 2016
 *  Modified: Abhishek Malik <abhishek.malik@intel.com>
 */
#include <stdarg.h>
#include <mraa/pwm.h>
#include "upm.h"

#ifndef UPM_SERVO_H_
#define UPM_SERVO_H_

struct _upm_servo_ft {
	upm_result_t (*upm_servo_set_angle) (void* dev, int32_t angle);
} upm_servo_func_table;

typedef struct servo_data_struct {
	uint32_t angle;
} servo_data_struct;

#endif /* UPM_SERVO_H_ */

