/*
 * Author: Jon Trulson <jtrulson@ics.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
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
#include "upm.h"
#include "mraa/i2c.h"

#define MPR121_I2C_BUS     0
#define MPR121_DEFAULT_I2C_ADDR    0x5a

/**
 * Opaque pointer for sensor struct
 */
typedef struct _upm_mpr121* upm_mpr121;

/**
 * Returns sensor descriptor
 *
 * @param dev sensor structure
 * @return upm_sensor_descriptor_t descriptor for the sensor
 */
const upm_sensor_descriptor_t upm_mpr121_get_descriptor ();

/**
 * Gets function table
 *
 * @param sensor_type type of ft requested
 * @return void* pointer to the function table
 */
const void* upm_mpr121_get_ft(upm_sensor_t sensor_type);

/**
 * Generic Init function
 *
 * @return void* pointer to the sensor struct
 */
void* upm_mpr121_init_name();

/**
 * MPR121 Init function
 *
 * @param bus i2c bus
 * @param address i2c address of the sensor
 * @return void* pointer to the sensor struct
 */
void* upm_mpr121_init(int bus, uint8_t address);

/**
 * MPR121 Close function
 *
 * @param dev pointer to sensor struct
 */
void upm_mpr121_close(void* dev);

/**
 * Sets up a default configuration, based on Application Note 3944
 * (AN3944):
 * http://cache.freescale.com/files/sensors/doc/app_note/AN3944.pdf
 *
 * After configuration, the sensor is left in the run state.
 *
 * @param dev pointer to the sensor struct
 * @return upm_result_t UPM success/error code
 */
upm_result_t upm_mpr121_config_an3944(void* dev);

/**
 * Function to read the individual button values
 *
 * @param dev pointer to the sensor struct
 * @param states Pointer to store the states of the individual buttons
 * @param num used to retrieve the state of a particular button
 * @return upm_result_t UPM success/error code
 */
upm_result_t upm_mpr121_read_buttons(void* dev, uint32_t* states, int num);

/**
 * Internal function used to write multiple bytes to the i2c bus
 *
 * @param dev sensor struct
 * @param reg i2c register to read from
 * @param *buffer pointer that contains the values that have to be
 * written to the registers
 * @param len length of the values that have to be written to the regs
 * @return upm_result_t UPM success/error code
 */
upm_result_t upm_mpr121_write_bytes(void* dev, uint8_t reg, uint8_t *buffer, int len);

/**
 * Internal function used to read multiple bytes from the i2c bus
 *
 * @param dev sensor struct
 * @param reg i2c register to read from
 * @param *buffer pointer that contains the values that have been
 * read from the registers
 * @param len length of the values that have to be read
 * @return upm_result_t UPM success/error code
 */

upm_result_t upm_mpr121_read_bytes(void* dev, uint8_t reg, uint8_t *buffer, int len);

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
upm_result_t upm_mpr121_read(const void* dev, void* value, int len);

/**
 * Generic write function for the sensor.
 *
 * @param void* pointer to the sensor struct
 * @param void* value stores the value to write
 * @param int len length of the elements of the
 * value to be written
 * @return upm_result_t UPM success/error code
 */
upm_result_t upm_mpr121_write(const void* dev, void* value, int len);

#endif /* MPR121_MPR121_H_ */
