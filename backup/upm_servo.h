/*
 * upm_servo.h
 *
 *  Created on: May 4, 2016
 *  Modified: Abhishek Malik <abhishek.malik@intel.com>
 */
#include <stdarg.h>
#include <mraa/pwm.h>
//#include "another.h"
#ifndef UPM_SERVO_H_
#define UPM_SERVO_H_
//#define UPM_SERVO_MOTOR es08a

struct _upm_servo_ft upm_servo_get_function_table();

struct _upm_servo_ft {
	//void* (*upm_servo_init) (int32_t pin, int32_t min_pulse_width, int32_t max_pulse_width);
	void* (*upm_servo_init) (int num,...);
	mraa_result_t (*upm_servo_close) (void* dev);
	mraa_result_t (*upm_servo_set_angle) (void* dev, int32_t angle);
	int32_t (*upm_servo_calc_pulse_travelling) (void* dev, int32_t value);
	void (*upm_servo_set_min_pulse_width) (void* dev, int width);
	void (*upm_servo_set_max_pulse_width) (void* dev, int width);
	int (*upm_servo_get_min_pulse_width) (void* dev);
	int (*upm_servo_get_max_pulse_width) (void* dev);
} func_table;

#endif /* UPM_SERVO_H_ */

