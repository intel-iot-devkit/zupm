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
#ifndef UPM_SENSOR_DISTANCE_H_
#define UPM_SENSOR_DISTANCE_H_
#include <stdarg.h>
#include "upm_utilities.h"

typedef enum upm_dist_units {RAW_DIST, CENTIMETER, INCH} upm_distance_units;
//typedef struct _upm_dist_ft* upm_distance_func_table;

struct _upm_dist_ft {
	void* (*upm_distance_init) (int num,...);
	upm_result_t (*upm_distance_close) (void* dev);
	upm_result_t (*upm_distance_get_distance) (void* dev, float* dist, upm_distance_units dist_unit);
} upm_distance_func_table;

#endif /* UPM_SENSOR_DISTANCE_H_ */
