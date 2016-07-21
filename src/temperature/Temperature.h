/*
 * Author: Sisinty Sasmita Patra <sisinty.s.patra@intel.com>
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

#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_

#pragma once
#include <stdlib.h>
#include <unistd.h>

#include "upm.h"
#include "upm_fti.h"
#include "mraa/aio.h"

typedef struct _upm_temperature* upm_temperature;

const upm_sensor_descriptor_t upm_temperature_get_descriptor();

const void* upm_temperature_get_ft(upm_sensor_t sensor_type);

void* upm_temperature_init(int pin);

void* upm_temperature_init_name();

void upm_temperature_close(void* dev);

upm_result_t upm_temperature_get_value_temperature(void* dev, float* tempval, upm_temperature_u unit);

#endif /* TEMPERATURE_H_ */
