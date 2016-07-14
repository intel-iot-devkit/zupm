/*
 * Author: Jon Trulson <jtrulson@ics.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
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
#ifndef MQ303A_MQ303A_H_
#define MQ303A_MQ303A_H_
#include "upm.h"
#include "mraa/aio.h"
/**
 * @brief MQ303A Alcohol Sensor library
 * @defgroup mq303a libupm-mq303a
 * @ingroup analog gpio gaseous tsk
 */
/**
 * @library mq303a
 * @sensor mq303a
 * @comname MQ303A Alcohol Sensor
 * @type gaseous
 * @man seeed
 * @web http://www.seeedstudio.com/document/pdf/MQ303A.pdf
 * @con analog gpio
 * @kit tsk
 *
 * @brief API for the MQ303A Alcohol Sensor
 *
 * UPM module for the MQ303A alcohol sensor.
 * This sensor needs to be warmed up before stable results are
 * obtained. The higher the value returned from value(),
 * the higher the amount of alcohol detected.
 *
 * @image html mq303a.jpg
 * @snippet mq303a.cxx Interesting
 */

/*
 * Opaque pointer to the sensor structure
 */
typedef struct _upm_mq303a* upm_mq303a;

const upm_sensor_descriptor_t upm_mq303a_get_descriptor();
/**
 * Gets the function tables associated with the sensor
 *
 * @param sensor_type the type of sensor
 * required for the ft to be returned
 * @return void* pointer to the ft struct
 */
const void* upm_mq303a_get_ft(upm_sensor_t sensor_type);

/**
 * MQ303A Name Initialization function
 *
 * @return void* pointer to the sensor struct
 */
void* upm_mq303a_init_name();

/**
 * MQ303A Initialization function
 *
 * @param pin Analog pin to use
 * @param heaterPin Digital pin mapped to the analog pin to use
 * @return void* pointer to the sensor struct
 */
void* upm_mq303a_init(int pin, int heater_pin);

/**
 * MQ303A Initialization function
 *
 * @param void* pointer to the sensor struct
 */
void upm_mq303a_close(void* dev);

/**
 * This function gets the value of the alcohol content
 *
 * @param void* pointer to the sensor struct
 * @param *val pointer to store the reading
 * @return upm_result_t UPM success/error code
 */
upm_result_t upm_mq303a_get_value(void* dev, int* val);

/**
 * This function enables/disables the heater
 * on board the sensor.
 *
 * @param void* pointer to the sensor struct
 * @param bool to turn on/off heater
 * @return upm_result_t UPM success/error code
 */
upm_result_t upm_mq303a_heater_enable(void* dev, bool enable);

#endif /* MQ303A_MQ303A_H_ */
