/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 *            Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef ES9257_H_
#define ES9257_H_

#pragma once

#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "upm.h"
#include "mraa/pwm.h"
//#include "mraa/pwm.h"
//#include "types/upm_sensor.h"

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
#define UPM_ES9257_MAX_ANGLE        180.0

#define UPM_ES9257_HIGH             1
#define UPM_ES9257_LOW              0

/*
 * Opaque pointer to the servo motor struct
 */
//struct _upm_es9257;
typedef struct _upm_es9257* upm_es9257;

/**
 * Instantiates a the servo at the given pin
 *
 * @param pin Servo pin number
 * @param minPulseWidth Minimum pulse width, in microseconds
 * @param maxPulseWidth Maximum pulse width, in microseconds
 * @param waitAndDisablePwm If 1, PWM is enabled only during the setAngle() execution
 * for a period of 1 second, and then turned back off. If 0, PWM remains on afterward.
 */

const upm_sensor_descriptor_t upm_es9257_get_descriptor ();

const void* upm_es9257_get_ft(upm_sensor_t sensor_type);
//upm_es9257 upm_es9257_init(int32_t pin, int32_t min_pulse_width, int32_t max_pulse_width);
//void* upm_es9257_init(int32_t pin, int32_t min_pulse_width, int32_t max_pulse_width);
void* upm_es9257_init_name();

void* upm_es9257_init(int32_t pin, int32_t min_pulse_width, int32_t max_pulse_width);

/**
 * Halts PWM for this servo and allows it to move freely.
 */
void upm_es9257_close(void* dev);

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
upm_result_t upm_es9257_calc_pulse_travelling(const void* dev,
                                              int32_t* ret_val,
                                              int32_t value);

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

#endif /* ES9257_H_ */
