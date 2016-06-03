/*
 * Authors: 
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
#ifndef _UPM_PH_H_
#define _UPM_PH_H_

#include <stdarg.h>
#include "upm.h"
#include "mraa/types.h"

/**
 * pH sensor value types
 */
typedef enum {
    UPM_PH_RAW = 0,
    UPM_PH_NORMALIZED,
    UPM_PH_PH
} upm_ph_value_t;


// pH sensor function table
typedef struct upm_ft_ph {
    void (*get_upm_descriptor) (upm_sensor_descriptor* desc);
    void* (*upm_ph_init) (int pin, float aref);
    upm_result_t (*upm_ph_close) (void* dev);
    upm_result_t (*upm_ph_set_offset) (void* dev, float ph_offset);
    upm_result_t (*upm_ph_get_value) (void* dev, float* value, upm_ph_value_t unit);
} upm_ft_ph;

// pH sensor helper methods
typedef upm_ft_ph (*func_get_upm_ft_ph)();

#endif /* _UPM_PH_H_ */
