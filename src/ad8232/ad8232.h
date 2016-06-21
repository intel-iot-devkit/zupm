/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * 		   Abhishek Malik <abhishek.malik@intel.com>
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

#ifndef AD8232_H_
#define AD8232_H_

#pragma once
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../upm.h"

#define AD8232_DEFAULT_AREF  3.3

typedef struct _upm_ad8232* upm_ad8232;

upm_sensor_descriptor_t upm_ad8232_get_descriptor (void* dev);

void* upm_ad8232_get_ft(upm_sensor_t sensor_type);

void* upm_ad8232_init_name(int num,...);

void* upm_ad8232_init(int lo_plus, int lo_minus, int output, float a_ref);

void upm_ad8232_close(void* dev);

upm_result_t upm_ad8232_get_value(void* dev, int* value, upm_heart_rate_u rate_unit);

upm_result_t upm_ad8232_read(void* dev, void* data, int* len);

upm_result_t upm_ad8232_write(void* dev, void* value, int len);

#endif /* AD8232_H_ */
