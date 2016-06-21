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

#ifndef UPM_C_GROVEMOISTURE_GROVEMOISTURE_H_
#define UPM_C_GROVEMOISTURE_GROVEMOISTURE_H_

#pragma once
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../upm.h"

typedef struct _upm_grove_moisture* upm_grove_moisture;

upm_sensor_descriptor_t upm_grove_moisture_get_descriptor (void* dev);

void* upm_grove_moisture_get_ft(upm_sensor_t sensor_type);

void* upm_grove_moisture_init_name(int num,...);

void* upm_grove_moisture_init(int pin);

void upm_grove_moisture_close(void* dev);

upm_result_t upm_grove_moisture_get_moisture(void* dev, int* moisture);

upm_result_t upm_grove_moisture_read(void* dev, void* data, int* len);

upm_result_t upm_grove_moisture_write(void* dev, void* data, int len);

#endif /* UPM_C_GROVEMOISTURE_GROVEMOISTURE_H_ */
