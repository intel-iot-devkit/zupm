/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Contributions: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Contributions: Sarah Knepper <sarah.knepper@intel.com>
 *                Abhishek Malik <abhishek.malik@intel.com>
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
#ifndef LED_LED_H_
#define LED_LED_H_
#include "upm.h"
#include "upm_fti.h"
#include "mraa/gpio.h"

/**
 * @type led
 * @con gpio
 * @kit gsk
 *
 * @brief API for the Grove LED
 *
 * UPM module for the Grove LED (or other similar light-emitting diodes).
 * An LED is a small lightbulb that emits light in
 * response to a small current. The longer wire of an LED connects
 * to the positive seat (anode); the shorter wire connects to the
 * negative seat (cathode). The flat side of the bulb corresponds
 * to the cathode, while the rounded side corresponds to the anode.
 *
 * @image html groveled.jpg
 */

/**
 * Opaque pointer to the sensor structure
 */
typedef struct _upm_led* upm_led;

const upm_sensor_descriptor_t upm_led_get_descriptor();

/**
 * Gets the function tables associated with the sensor
 *
 * @param sensor_type the type of sensor
 * required for the ft to be returned
 * @return void* pointer to the ft struct
 */
const void* upm_led_get_ft(upm_sensor_t sensor_type);

/**
 * LED Name Initialization function
 *
 * @return void* pointer to the sensor struct
 */
void* upm_led_init_name();

/**
 * LED Initialization function
 *
 * @param pin GPIO pin to use
 * @return void* pointer to the sensor struct
 */
void* upm_led_init(uint8_t pin);

/**
 * MQ303A Initialization function
 *
 * @param void* pointer to the sensor struct
 */
void upm_led_close(void* dev);

/**
 * Function to turn LED on
 *
 * @param void* pointer to the sensor struct
 * @return upm_result_t UPM success/error code
 */
upm_result_t upm_led_on(void* dev);

/**
 * Function to turn LED off
 *
 * @param void* pointer to the sensor struct
 * @return upm_result_t UPM success/error code
 */
upm_result_t upm_led_off(void* dev);

#endif /* LED_LED_H_ */
