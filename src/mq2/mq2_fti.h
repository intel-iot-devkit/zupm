/*
 * Author:
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

#pragma once

#include "mq2.h"

#include "upm_fti.h"
#include "types/upm_sensor.h"
#include "types/upm_raw.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
 * Get the generic function table for this sensor
 * @return generic function table struct
 */
const void* upm_mq2_get_ft(upm_sensor_t sensor_type);

/**
 * Initialize analog sensor
 * @param protocol protocol initliazation string
 * @param params sensor parameter string
 * @return sensor context as void pointer
 */
void* upm_mq2_init_str(const char* protocol, const char* params);

/**
 * Analog sensor destructor
 * @param sensor context pointer deallocate memory
 */
void upm_mq2_close(void* dev);

/**
 * Get descriptor for this sensor
 * @return Sensor descriptor struct
 */
const upm_sensor_descriptor_t upm_mq2_get_descriptor();

/**
 * Set sensor offset.  This offset is applied to the return value:
 *     counts = counts + offset
 * @param dev sensor context pointer
 * @param offset count offset value used
 * @return Function result code
 */
upm_result_t upm_mq2_set_offset(const void* dev, float offset);

/**
 * Set sensor scale.  This scale is applied to the return value:
 *     counts = counts * scale
 * @param dev sensor context pointer
 * @param scale count scale value used
 * @return Function result code
 */
upm_result_t upm_mq2_set_scale(const void* dev, float scale);

/**
 * Read value from sensor
 * @param dev sensor context pointer
 * @param *value counts value from sensor
 * @return Function result code
 */
upm_result_t upm_mq2_get_value(const void* dev, float *value);

#ifdef __cplusplus
}
#endif
