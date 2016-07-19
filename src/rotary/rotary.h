/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * 	   Sisinty Sasmita Patra <sisinty.s.patra@intel.com>
 *
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

#ifndef ROTARY_H_
#define ROTARY_H_

#pragma once
#include <stdlib.h>
#include <unistd.h>

#include <mraa/aio.h>

#include "upm.h"
#include "upm_fti.h"

#define UPM_ROTARY_MAX_ANGLE 300

typedef struct _upm_rotary* upm_rotary;

void* upm_rotary_init(int pin, float aRef);

void* upm_rotary_init_name(char* protocol, char* params);

void upm_rotary_close(void* dev);

upm_result_t upm_rotary_read(void* dev, void *value, int len);

upm_result_t upm_rotary_write(void* dev, void *value, int len);

upm_result_t upm_rotary_get_value_voltage(void* dev, float* rotval, upm_voltage_u unit);

upm_result_t upm_rotary_get_value_angle(void* dev, float* rotval, upm_angle_u unit);

#endif /* ROTARY_H_ */
