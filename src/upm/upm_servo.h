/*
 * upm_servo.h
 *
 *  Created on: May 4, 2016
 *  Modified: Abhishek Malik <abhishek.malik@intel.com>
 */
#include <stdarg.h>
#include <mraa/pwm.h>
#include "upm_utilities.h"

#ifndef UPM_SERVO_H_
#define UPM_SERVO_H_

struct _upm_servo_ft {
	void* (*upm_servo_init) (int num,...);
	upm_result_t (*upm_servo_close) (void* dev);
	upm_result_t (*upm_servo_set_angle) (void* dev, int32_t angle);
} upm_servo_func_table;

#endif /* UPM_SERVO_H_ */

