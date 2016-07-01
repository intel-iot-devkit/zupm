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
#include "types/upm_sensor.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Get the generic function table for this sensor
 * @return generic function table struct
 */
const void* upm_joystick12_get_ft(upm_sensor_t sensor_type);

/**
 * Initialize analog sensor
 * @param protocol protocol initliazation string
 * @param params sensor parameter string
 * @return sensor context as void pointer
 */
void* upm_joystick12_init_str(const char* protocol, const char* params);

/**
 * Initialize analog sensor
 * @param pin is Analog pin
 * @return sensor context as void pointer
 */
void* upm_joystick12_init(int16_t apin_x, int16_t apin_y);

/**
 * Analog sensor destructor
 * @param sensor context pointer deallocate memory
 */
void upm_joystick12_close(void* dev);

/**
 * Get descriptor for this sensor
 * @return Sensor descriptor struct
 */
const upm_sensor_descriptor_t upm_joystick12_get_descriptor();

/**
 * Generic sensor read method
 * @param dev sensor device context pointer
 * @param value pointer to value read from hardware
 * @param len length of items to read
 */
upm_result_t upm_joystick12_read(const void* dev, void* value, int len);

/**
 * Generic sensor write method
 * @param dev sensor device context pointer
 * @param value pointer to value to write
 * @param len length of items to write
 */
upm_result_t upm_joystick12_write(const void* dev, void* value, int len);

/**
 * Set sensor offset.  This offset is applied to the return value:
 *     counts = counts + offset
 * @param dev sensor context pointer
 * @param offset count offset value used
 * @return Function result code
 */
upm_result_t upm_joystick12_set_offset_x(const void* dev, float offset);

/**
 * Set sensor offset.  This offset is applied to the return value:
 *     counts = counts + offset
 * @param dev sensor context pointer
 * @param offset count offset value used
 * @return Function result code
 */
upm_result_t upm_joystick12_set_offset_y(const void* dev, float offset);

/**
 * Set sensor scale.  This scale is applied to the return value:
 *     counts = counts * scale
 * @param dev sensor context pointer
 * @param scale count scale value used
 * @return Function result code
 */
upm_result_t upm_joystick12_set_scale_x(const void* dev, float scale);

/**
 * Set sensor scale.  This scale is applied to the return value:
 *     counts = counts * scale
 * @param dev sensor context pointer
 * @param scale count scale value used
 * @return Function result code
 */
upm_result_t upm_joystick12_set_scale_y(const void* dev, float scale);

/**
 * Read value from sensor
 * @param dev sensor context pointer
 * @param *value normalized value from sensor, units depend on unit enum
 * @param unit Enum which specifies units returned in *value
 * @return Function result code
 */
upm_result_t upm_joystick12_get_value_x(const void* dev, float *value);

/**
 * Read value from sensor
 * @param dev sensor context pointer
 * @param *value normalized value from sensor, units depend on unit enum
 * @param unit Enum which specifies units returned in *value
 * @return Function result code
 */
upm_result_t upm_joystick12_get_value_y(const void* dev, float *value);

/**
 * Zero out the sensor x and y axis
 * @param dev sensor context pointer
 * @return Function result code
 */
upm_result_t upm_joystick12_zero(const void* dev);

/**
 * Set x scale.  Move joystick to min or max x and call calibrate
 * @param dev sensor context pointer
 * @return Function result code
 */
upm_result_t upm_joystick12_calibrate_x(const void* dev);

/**
 * Set y scale.  Move joystick to min or max y and call calibrate
 * @param dev sensor context pointer
 * @return Function result code
 */
upm_result_t upm_joystick12_calibrate_y(const void* dev);

#ifdef __cplusplus
}
#endif
