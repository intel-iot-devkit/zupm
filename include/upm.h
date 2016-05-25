/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Contributions: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
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
#pragma once

#define upm_sensor_category_invalid 0
#define upm_sensor_category_size 5

/* Sensor categories (all sensors have *at least* 1) */
typedef enum {
    UPM_INVALID = 0,
    UPM_PH,
    UPM_PRESSURE,
    UPM_SERVO,
    UPM_TEMPERATURE
} upm_sensor_category;

/* Sensor descriptor */
typedef struct upm_sensor_descriptor {
    char name[20];
    char description[100];
    upm_sensor_category category[upm_sensor_category_size];
} upm_sensor_descriptor;

/* UPM helper methods */
typedef void (*func_get_upm_descriptor)(upm_sensor_descriptor* desc);

inline const char * to_char(upm_sensor_category category)
{
    switch(category) {
        case UPM_PH:
            return "PH";
            break;
        case UPM_PRESSURE:
            return "PRESSURE";
            break;
        case UPM_SERVO:
            return "SERVO";
            break;
        case UPM_TEMPERATURE:
            return "TEMPERATURE";
            break;
        default:
            return "INVALID";
    }
}

/**
 * UPM return codes, copied from MRAA
 * TODO: Find a better solution here
 */
typedef enum {
    UPM_SUCCESS = 0,                             /**< Expected response */
    UPM_ERROR_FEATURE_NOT_IMPLEMENTED = 1,       /**< Feature TODO */
    UPM_ERROR_FEATURE_NOT_SUPPORTED = 2,         /**< Feature not supported by HW */
    UPM_ERROR_INVALID_VERBOSITY_LEVEL = 3,       /**< Verbosity level wrong */
    UPM_ERROR_INVALID_PARAMETER = 4,             /**< Parameter invalid */
    UPM_ERROR_INVALID_HANDLE = 5,                /**< Handle invalid */
    UPM_ERROR_NO_RESOURCES = 6,                  /**< No resource of that type avail */
    UPM_ERROR_INVALID_RESOURCE = 7,              /**< Resource invalid */
    UPM_ERROR_INVALID_QUEUE_TYPE = 8,            /**< Queue type incorrect */
    UPM_ERROR_NO_DATA_AVAILABLE = 9,             /**< No data available */
    UPM_ERROR_INVALID_PLATFORM = 10,             /**< Platform not recognised */
    UPM_ERROR_PLATFORM_NOT_INITIALISED = 11,     /**< Board information not initialised */
    UPM_ERROR_UART_OW_SHORTED = 12,              /**< UART OW Short Circuit Detected*/
    UPM_ERROR_UART_OW_NO_DEVICES = 13,           /**< UART OW No devices detected */
    UPM_ERROR_UART_OW_DATA_ERROR = 14,           /**< UART OW Data/Bus error detected */

    UPM_ERROR_UNSPECIFIED = 99 /**< Unknown Error */
} upm_result_t;

#define upm_perror(...) perror(args, __VA_ARGS__)
