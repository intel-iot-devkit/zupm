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

#include "upm.h"

#ifdef __cplusplus
extern "C" {
#endif

// opaque context forward declaration
typedef struct _mq8_context *mq8_context;

/**
 * Initialize analog sensor
 * @param pin Analog pin
 * @return sensor context
 */
mq8_context mq8_init(int16_t pin);

/**
 * Analog sensor destructor
 * @param sensor context pointer
 */
void mq8_close(mq8_context dev);

/**
 * Read value from sensor
 * @param dev sensor context pointer
 * @param *value counts value from sensor
 * @return Function result code
 */
upm_result_t mq8_get_value(const mq8_context dev, float *value);

/**
 * Set sensor offset.  This offset is applied to the return value:
 *     counts = counts + offset
 * @param dev sensor context pointer
 * @param offset count offset value used
 * @return Function result code
 */
upm_result_t mq8_set_offset(const mq8_context dev, float offset);

/**
 * Set sensor scale.  This scale is applied to the return value:
 *     counts = counts * scale
 * @param dev sensor context pointer
 * @param scale count scale value used
 * @return Function result code
 */
upm_result_t mq8_set_scale(const mq8_context dev, float scale);


#ifdef __cplusplus
}
#endif
