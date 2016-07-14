/*
 * Author: Sarah Knepper <sarah.knepper@intel.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2015 Intel Corporation.
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

#ifndef UPM_C_TTP223_TTP223_H_
#define UPM_C_TTP223_TTP223_H_

#pragma once
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "upm.h"
#include "mraa/gpio.h"

/**
 * Opaque pointer to the sensor struct
 */
typedef struct _upm_ttp223* upm_ttp223;

/**
 * Fetches the descriptor for the sensor
 *
 * @param dev pointer to the sensor struct
 * @return upm_sensor_descriptor_t descriptor associated
 * with the sensor
 */
const upm_sensor_descriptor_t upm_ttp223_get_descriptor();

/**
 * Fetches the function tables associated with the sensor
 *
 * @param sensor_type the type of sensor
 * @return void* pointer to the function table requested
 */
const void* upm_ttp223_get_ft(upm_sensor_t sensor_type);

/**
 * Generic init function
 *
 * @return void* pointer to the sensor struct
 */
void* upm_ttp223_init_name();

/**
 * Sensor Init function
 *
 * @param pin The pin number the sensor is attached to
 * @return void* pointer to the sensor struct
 */
void* upm_ttp223_init(int pin);

/**
 * Sensor Module close function
 *
 * @param dev pointer to the sensor struct
 */
void upm_ttp223_close(void* dev);

/**
 * Function to tell if the sensor is pressed
 *
 * @param dev pointer to the sensor struct
 * @param value pointer to store whether the sensor is pressed or not
 * @param num the touchpad pressed
 * @result upm_result_t UPM success/error code
 */
upm_result_t upm_ttp223_is_pressed(void* dev, bool* value, int num);

/**
 *
 */
upm_result_t upm_ttp223_install_isr(void* dev, mraa_gpio_edge_t edge_level, void (*isr)(void *), void *arg);

/**
 *
 */
upm_result_t upm_ttp223_uninstall_isr(void* dev);

#endif /* UPM_C_TTP223_TTP223_H_ */
