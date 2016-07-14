/*
 * Author: Jon Trulson <jtrulson@ics.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
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

#ifndef UPM_C_GROVEMOISTURE_GROVEMOISTURE_H_
#define UPM_C_GROVEMOISTURE_GROVEMOISTURE_H_

#pragma once
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "upm.h"
#include "mraa/aio.h"

/**
 * Opaque pointer to the sensor struct
 */
typedef struct _upm_grove_moisture* upm_grove_moisture;

/**
 * Gets the descriptor associated with the sensor driver
 *
 * @param void* pointer to the sensor struct
 * @raturn upm_sensor_descriptor_t descriptor type
 */
const upm_sensor_descriptor_t upm_grove_moisture_get_descriptor ();

/**
 * Gets the function tables associated with the sensor
 *
 * @param sensor_type the type of sensor
 *         required for the ft to be returned
 * @return void* pointer to the ft struct
 */
const void* upm_grove_moisture_get_ft(upm_sensor_t sensor_type);

/**
 * Grove Moisture Name Initialization function
 *
 * @return void* pointer to the sensor struct
 */
void* upm_grove_moisture_init_name();

/**
 * Init function
 *
 * @param pin analog pin number
 * @return void* pointer to the sensor struct
 */
void* upm_grove_moisture_init(int pin);

/**
 * Close function
 *
 * @param dev pointer to the sensor structure
 */
void upm_grove_moisture_close(void* dev);

/**
 * Get Moisture function.
 *
 * @param dev pointer to the sensor struct
 * @param moisture pointer that will be used to store the
 * output value from the sensor
 */
upm_result_t upm_grove_moisture_get_moisture(void* dev, int* moisture);

#endif /* UPM_C_GROVEMOISTURE_GROVEMOISTURE_H_ */
