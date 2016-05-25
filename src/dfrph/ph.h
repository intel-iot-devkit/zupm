/*
 * Author: Sisinty Sasmita Patra <sisinty.s.patra@intel.com>
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

#include <mraa/aio.h>

#include "upm.h"
#include "upm_sensor_ph.h"

/**
 * pH sensor Struct
 */
struct _upm_ph {
    mraa_aio_context aio;
    int16_t m_aRes;
    float m_aRef;
    float m_offset;
};

typedef struct _upm_ph* upm_ph;

/**
 * Fill in a sensor descriptor structure for this sensor
 * @param desc Sensor descriptor struct
 * @return void
 */
void get_upm_descriptor(upm_sensor_descriptor* desc);

/**
 * Get the function table for this sensor
 * @return pH function table struct
 */
upm_ft_ph upm_get_ph_ft();

/**
 * Initialize pH sensor
 * @param pin is Analog pin
 * @return upm_ph struct
 */
upm_ph upm_dfrph_init(int16_t pin, float aRef);

/**
 * pH sensor destructor
 * @param upm_ph dev struct to deallocate memory
 */
void upm_dfrph_close(upm_ph dev);

/**
 * Set a +/- pH offset.  This offset is applied to the return pH
 *     pH = pH + offset
 * @param upm_ph dev struct
 * @param ph_offset pH offset value used
 * @return Function result code
 */
upm_result_t upm_dfrph_set_offset(upm_ph dev, float ph_offset);

/**
 * Read value from sensor
 * @param upm_ph dev struct
 * @param *value pH value from sensor, units depend on 
 * @param unit Enum which specifies units for value return in value
 * @return Function result code
 */
upm_result_t upm_dfrph_get_value(upm_ph dev, float *value, upm_ph_value_t unit);
