/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * 		   Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2014 Intel Corporation.
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

#ifndef MPR121_MPR121_H_
#define MPR121_MPR121_H_
#include "../upm.h"

#define MPR121_I2C_BUS     0
#define MPR121_DEFAULT_I2C_ADDR    0x5a

typedef struct _upm_mpr121* upm_mpr121;

upm_sensor_descriptor_t upm_mpr121_get_descriptor (void* dev);

void* upm_mpr121_get_ft(upm_sensor_t sensor_type);

void* upm_mpr121_init_name(int num,...);

void* upm_mpr121_init(int bus, uint8_t address);

void upm_mpr121_close(void* dev);

upm_result_t upm_mpr121_config_an3944(void* dev);

upm_result_t upm_mpr121_read_buttons(void* dev, uint32_t* states, int num);

upm_result_t upm_mpr121_write_bytes(void* dev, uint8_t reg, uint8_t *buffer, int len);

upm_result_t upm_mpr121_read_bytes(void* dev, uint8_t reg, uint8_t *buffer, int len);

upm_result_t upm_mpr121_read(void* dev, void* value, int* len);

upm_result_t upm_mpr121_write(void* dev, void* value, int len);

#endif /* MPR121_MPR121_H_ */
