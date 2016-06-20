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

#include <mraa/aio.h>

#include "upm.h"
#include "upm_sensor.h"
#include "upm_ph.h"

/**
 * pH sensor Struct
 */
typedef struct _upm_dfrph {
    /* mraa aio pin context */
    mraa_aio_context aio;
    /* Analog bit resolution (number of bits) */
    int16_t m_aRes;
    /* Analog voltage reference */
    float m_aRef;
    /* */
    float m_offset;
} upm_dfrph;

/**
 * Fill in a sensor descriptor structure for this sensor
 * @param desc Sensor descriptor struct
 * @return void
 */
void get_upm_descriptor(upm_sensor_descriptor_t* desc);

/**
 * Get the function table for this sensor
 * @return generic function table struct
 */
upm_sensor_ft upm_get_ft();

/**
 * Initialize pH sensor
 * @param protocol protocol initliazation string
 * @param params sensor parameter string
 * @return upm_dfrph struct pointer
 */
upm_dfrph* upm_dfrph_init_str (const char* protocol, const char* params);

/**
 * Initialize pH sensor
 * @param pin is Analog pin
 * @return upm_dfrph struct pointer
 */
upm_dfrph* upm_dfrph_init(int16_t pin, float aref);

/**
 * pH sensor destructor
 * @param upm_dfrph dev struct to deallocate memory
 */
void upm_dfrph_close (upm_dfrph* dev);

upm_result_t upm_dfrph_read (void* dev, void* value, int len);
upm_result_t upm_dfrph_write (void* dev, void* value, int len);
const upm_sensor_descriptor_t upm_dfrph_get_descriptor (void* dev);


/**
 * Set a +/- pH offset.  This offset is applied to the return pH
 *     pH = pH + offset
 * @param upm_dfrph dev struct
 * @param ph_offset pH offset value used
 * @return Function result code
 */
upm_result_t upm_dfrph_set_offset(upm_dfrph* dev, float ph_offset);

/**
 * Read value from sensor
 * @param upm_dfrph dev struct
 * @param *value pH value from sensor, units depend on 
 * @param unit Enum which specifies units for value return in value
 * @return Function result code
 */
upm_result_t upm_dfrph_get_value(upm_dfrph* dev, float *value, upm_ph_u unit);
