/*
 * es9257.h
 *
 *  Created on: May 6, 2016
 *  Modified: Abhishek Malik <abhishek.malik@intel.com>
 */

#ifndef ES9257_H_
#define ES9257_H_

#pragma once

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <mraa/pwm.h>
#include "../upm/upm_servo.h"

/**
 * @library servo
 * @sensor es9257
 * @comname ES9257 Servo
 * @altname Grove Servo
 * @type servos
 * @man emax
 * @web http://www.seeedstudio.com/wiki/Grove_-_Servo
 * @con pwm
 * @kit gsk
 *
 * @brief API for the ES9257 Servo
 *
 * This module defines the ES9257 interface for ES9257 servos.
 * Like other servos, the ES9257 servo has a shaft that can be controlled
 * by setting the desired angle. There are also routines for setting
 * and getting the minimum and maximum pulse width as well as the
 * maximum period.
 *
 * @image html es9257.jpg
 * @snippet es9257.c Interesting
 */

#define UPM_ES9257_MIN_PULSE_WIDTH  475
#define UPM_ES9257_MAX_PULSE_WIDTH  2100
#define UPM_ES9257_PERIOD           20000
#define UPM_ES9257_MAX_ANGLE		180.0

#define UPM_ES9257_HIGH             1
#define UPM_ES9257_LOW              0

/*
 * Opaque pointer to the servo motor struct
 */
//struct _upm_es9257;
typedef struct _upm_es9257* UpmES9257;

/**
 * Instantiates a the servo at the given pin
 *
 * @param pin Servo pin number
 * @param minPulseWidth Minimum pulse width, in microseconds
 * @param maxPulseWidth Maximum pulse width, in microseconds
 * @param waitAndDisablePwm If 1, PWM is enabled only during the setAngle() execution
 * for a period of 1 second, and then turned back off. If 0, PWM remains on afterward.
 */
//upm_es9257 upm_es9257_init(int32_t pin, int32_t min_pulse_width, int32_t max_pulse_width);
//void* upm_es9257_init(int32_t pin, int32_t min_pulse_width, int32_t max_pulse_width);
void* upm_es9257_init(int num,...);

/**
 * Halts PWM for this servo and allows it to move freely.
 */
upm_result_t upm_es9257_halt(void* dev);

/**
 * Sets the angle of the servo engine.
 *
 * @param angle Number between 0 and 180
 * @return 0 if successful, non-zero otherwise
 */
upm_result_t upm_es9257_set_angle(void* dev, int32_t angle);

/*
 * Calculating relative pulse time to the value.
 * */
upm_result_t upm_es9257_calc_pulse_travelling(void* dev, int32_t* ret_val, int32_t value);

/**
 * Sets the minimum pulse width
 *
 * @param width Minimum HIGH signal width
 */
void upm_es9257_set_min_pulse_width (void* dev, int width);

/**
 * Sets the maximum pulse width
 *
 * @param width Maximum HIGH signal width
 */
void upm_es9257_set_max_pulse_width (void* dev, int width);

/**
 * Returns the minimum pulse width
 *
 * @return Minimum pulse width
 */
int upm_es9257_get_min_pulse_width (void* dev);

/**
 * Returns the maximum pulse width
 *
 * @return Maximum pulse width
 */
int upm_es9257_get_max_pulse_width (void* dev);

void UpmES9257GetSensorInfo();

#endif /* ES9257_H_ */
