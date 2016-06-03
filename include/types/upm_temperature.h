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
#ifndef UPM_TEMPERATURE_H_
#define UPM_TEMPERATURE_H_
#include <stdarg.h>
#include "upm_utilities.h"

typedef enum upm_temp_units {RAW_TEMP, CELSIUS, FAHRENHEIT, KELVIN} upm_temperature_units;
//typedef struct _upm_temp_ft* upm_temperature_func_table;

struct _upm_temp_ft {
	void* (*upm_temperature_init) (int num,...);
	upm_result_t (*upm_temperature_close) (void* dev);
	upm_result_t (*upm_temperature_get_temperature) (void* dev, float* temp, upm_temperature_units temp_unit);
} upm_temperature_func_table;

#endif /* UPM_TEMPERATURE_H_ */
