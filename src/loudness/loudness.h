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

#ifndef LOUDNESS_LOUDNESS_H_
#define LOUDNESS_LOUDNESS_H_

#include "upm.h"
#include "mraa/aio.h"
/**
 * Opaque pointer to the sensor structure
 */
typedef struct _upm_loudness* upm_loudness;

const void* upm_loudness_get_ft(upm_sensor_t sensor_type);
const upm_sensor_descriptor_t upm_loudness_get_descriptor();

/**
 * Loudness Name Initialization function
 *
 * @return void* pointer to the sensor struct
 */
void* upm_loudness_init_name();

/**
 * Loudness Initialization function
 *
 * @param pin AIO pin to use
 * @return void* pointer to the sensor struct
 */
void* upm_loudness_init(int pin);

/**
 * Loudness Initialization function
 *
 * @param void* pointer to the sensor struct
 */
void upm_loudness_close(void* dev);

/**
 * Function gets the loudness raw value (currently)
 *
 * @param void* pointer to the sensor struct
 * @param int* pointer to store the loudness value.
 * @return upm_result_t UPM success/error code
 */
upm_result_t upm_loudness_get_value(void* dev, int* val);

/**
 * Generic read function for the sensor. Returns
 * raw value.
 *
 * @param void* pointer to the sensor struct
 * @param void* value stores the value that was read
 * @param int len length of the elements of the
 * value that has been read
 * @return upm_result_t UPM success/error code
 */
upm_result_t upm_loudness_read(const void* dev, void* value, int len);

/**
 * Generic write function for the sensor.
 *
 * @param void* pointer to the sensor struct
 * @param void* value stores the value to write
 * @param int len length of the elements of the
 * value to be written
 * @return upm_result_t UPM success/error code
 */
upm_result_t upm_loudness_write(const void* dev, void* value, int len);

#endif /* LOUDNESS_LOUDNESS_H_ */
