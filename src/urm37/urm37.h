/*
 * Author: Jon Trulson <jtrulson@ics.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * Thanks to Adafruit for supplying a google translated version of the
 * Chinese datasheet and some clues in their code.
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

#ifndef URM37_H_
#define URM37_H_

#pragma once
#include "upm.h"
#include "upm_fti.h"
#include "mraa/aio.h"
#include "mraa/uart.h"

#define URM37_DEFAULT_UART          0
#define A_REF                       5.0
#define UPM_URM37_MAX_CMD_LEN       4
#define UPM_URM37_MAX_RESP_LEN      4
#define UPM_URM37_WAIT_TIMEOUT      1000
#define UPM_URM37_MAX_RETRIES       1

/**
 * @brief DFRobot URM37 Ultrasonic Ranger
 * @defgroup urm37 libupm-urm37
 * @ingroup dfrobot uart gpio ainput sound
 */

/**
 * @library urm37
 * @sensor urm37
 * @comname DFRobot URM37 Ultrasonic Ranger
 * @type sound
 * @man dfrobot
 * @con uart ainput gpio
 * @web http://www.dfrobot.com/index.php?route=product/product&product_id=53
 *
 * @brief API for the DFRobot URM37 Ultrasonic Ranger
 *
 * The driver was tested with the DFRobot URM37 Ultrasonic Ranger,
 * V4.  It has a range of between 5 and 500 centimeters (cm).  It
 * supports both analog distance measurement, and UART based
 * temperature and distance measurements.  This driver does not
 * support PWM measurement mode.
 *
 * For UART operation, the only supported baud rate is 9600.  In
 * addition, you must ensure that the UART TX/RX pins are
 * configured for TTL operation (the factory default) rather than
 * RS232 operation, or permanent damage to your URM37 and/or MCU
 * will result.  On power up, the LED indicator will blink one
 * long pulse, followed by one short pulse to indicate TTL
 * operation.  See the DFRobot wiki for more information:
 *
 * (https://www.dfrobot.com/wiki/index.php?title=URM37_V4.0_Ultrasonic_Sensor_%28SKU:SEN0001%29)
 *
 * @image html urm37.jpg
 * An example using analog mode
 * @snippet urm37.cxx Interesting
 * An example using UART mode
 * @snippet urm37-uart.cxx Interesting
 */
typedef struct _upm_urm37v4* upm_urm37;;

const void* upm_urm37_get_ft(upm_sensor_t sensor_type);

void* upm_urm37_init_name();

/**
 * URM37 Initializer
 *
 * @param a_pin Analog pin to use
 * @param reset_pin GPIO pin to use for reset
 * @param trigger_pin GPIO pin to use for triggering a distance measurement
 * @param a_ref The analog reference voltage, default 5.0
 * @param uart Default UART to use (0 or 1).
 * @param mode analog/uart mode
 */
void* upm_urm37_init(uint8_t a_pin, uint8_t reset_pin, uint8_t trigger_pin, float a_ref, uint8_t uart, upm_protocol_t mode);

/**
 * URM37 sensor close function
 */
void upm_urm37_close(void* dev);

/**
 * Reset the device.  This will take approximately 3 seconds to
 * complete.
 *
 * @param dev sensor struct
 */
upm_result_t upm_urm37_reset(void* dev);

/**
 * Get the distance measurement.  A return value of 65535.0
 * in UART mode indicates an invalid measurement.
 *
 * @param dev sensor struct
 * @param distance return value for distance measured
 * @param dist_unit The measured distance in cm
 */
upm_result_t upm_urm37_get_distance(void* dev, float* distance, upm_distance_u dist_unit);

/**
 * Get the temperature measurement.  This is only valid in UART mode.
 *
 * @param dev sensor struct
 * @param temperature The measured temperature in degrees C
 * @param temp_unit unit of temp in C/F/K
 *
 */
upm_result_t upm_urm37_get_temperature(void* dev, float* temperature, upm_temperature_u temp_unit);


upm_result_t upm_urm37_get_mode(void* dev, upm_protocol_t* mode);

upm_result_t upm_urm37_set_mode(void* dev, upm_protocol_t mode);

/**
 * In UART mode only, read a value from the EEPROM and return it.
 *
 * @param dev sensor struct
 * @param addr The address in the EEPROM to read.  Valid values
 * are between 0x00-0x04.
 * @param return parameter for the EEPROM value at addr
 */
upm_result_t upm_urm37_read_EEPROM(void* dev, uint8_t addr, uint8_t* value);

/**
 * In UART mode only, write a value into an address on the EEPROM.
 *
 * @param dev sensor struct
 * @param addr The address in the EEPROM to write.  Valid values
 * are between 0x00-0x04.
 * @param value The value to write
 */
upm_result_t upm_urm37_write_EEPROM(void* dev, uint8_t addr, uint8_t value);

// send a serial command and return a 4 byte response (UART mode only)
upm_result_t upm_urm37_send_command(void* dev, char* cmd, char* response, int len);

//upm_result_t upm_urm37_data_available(void* dev, uint32_t millis, mraa_boolean_t* data_avail);

/**
 * Reads any available data and returns it in a std::string. Note:
 * the call blocks until data is available for reading. Use
 * dataAvailable() to determine whether there is data available
 * beforehand, to avoid blocking.
 *
 * @param dev sensor struct
 * @param len Maximum length of the data to be returned
 * @param data The data read
 */
upm_result_t upm_urm37_read_data_string(void* dev, uint32_t len, char* data);

/**
 * Writes the std:string data to the device.  If you are writing a
 * command, be sure to terminate it with a carriage return (\r)
 *
 * @param dev sensor struct
 * @param data_w Buffer to write to the device
 * @param len Number of bytes written
 */
upm_result_t upm_urm37_write_data_string(void* dev, const char* data_w, int len);

upm_result_t upm_urm37_get_degrees(void* dev, int* degrees);

upm_result_t upm_urm37_set_degrees(void* dev, int degrees);

#endif /* URM37_H_ */
