/*
 * Author: Zion Orent <sorent@ics.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
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
#ifndef MVS0608_MVS0608_H_
#define MVS0608_MVS0608_H_
#include "upm.h"
#include "upm_fti.h"
#include "mraa/gpio.h"
/**
 * @brief MVS0608 - Grove Collision Sensor library
 * @ingroup gpio accelerometer
 */

/**
 * @library mvs0608
 * @sensor mvs0608
 * @comname Grove Collision Sensor
 * @type accelerometer
 * @con gpio
 *
 * @brief API for the mvs0608
 *
 * MVS0608 can detect whether any
 * collision movement or vibration happens.
 * It outputs a low pulse signal when vibration is detected.
 *
 * @image html grovecollision.jpg
 * @snippet grovecollision.cxx Interesting
 */

/**
 * Opaque pointer to the sensor structure
 */
typedef struct _upm_mvs0608* upm_mvs0608;

const upm_sensor_descriptor_t upm_mvs0608_get_descriptor();

const void* upm_mvs0608_get_ft(upm_sensor_t sensor_type);

/**
 * MVS0608 Name Initialization function
 *
 * @return void* pointer to the sensor struct
 */
void* upm_mvs0608_init_name();

/**
 * MVS0608 Initialization function
 *
 * @param pin number
 * @return void* pointer to the sensor struct
 */
void* upm_mvs0608_init(int pin);

/**
 * MVS0608 Initialization function
 *
 * @param void* pointer to the sensor struct
 */
void upm_mvs0608_close(void* dev);

/**
 * This function tells you whether the sensor has
 * collided with something or not.
 *
 * @param void* pointer to the sensor struct
 * @param bool* pointer to hold the collision value
 * @return upm_result_t UPM success/error code
 */
upm_result_t upm_mvs_is_colliding(void* dev, bool* collision_val);

/**
 * Generic read function for the sensor. Returns
 * raw value.
 *
 * @param void* pointer to the sensor struct
 * @param void* value stores the value that was read
 * @param int len length of the elements of the
 * value that has been read
 * @return upm_result_t UPM success/error code
 */
upm_result_t upm_mvs0608_read(const void* dev, void* value, int len);

/**
 * Generic write function for the sensor.
 *
 * @param void* pointer to the sensor struct
 * @param void* value stores the value to write
 * @param int len length of the elements of the
 * value to be written
 * @return upm_result_t UPM success/error code
 */
upm_result_t upm_mvs0608_write(const void* dev, void* value, int len);

#endif /* MVS0608_MVS0608_H_ */
