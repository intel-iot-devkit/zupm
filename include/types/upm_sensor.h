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
#ifndef UPM_SENSOR_H_
#define UPM_SENSOR_H_

// Generic sensor function table
typedef struct _upm_sensor_ft {
    void* (*upm_sensor_init_name) (char* protocol, char* params);
    void (*upm_sensor_close) (void* dev);
    upm_result_t (*upm_sensor_read) (void* dev, void* value, int len);
    upm_result_t (*upm_sensor_write) (void* dev, void* value, int len);
    upm_sensor_descriptor_t (*upm_sensor_get_descriptor) (void* dev);
} upm_sensor_ft;

#endif /* UPM_SENSOR_H_ */

