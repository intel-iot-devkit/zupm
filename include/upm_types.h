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
#ifndef UPM_TYPES_H_
#define UPM_TYPES_H_

#ifdef C99
#include <stdbool.h>
#else
typedef enum {
    false = 0,
    true  = 1,
} bool;
#endif

typedef enum {
	UPM_SUCCESS,						/* Operation is successful */
	UPM_MODE_NOT_SUPPORTED,				/* Trying to access one mode while working on another mode */
	UPM_ERROR_UNSPECIFIED,				/* Unspecified error */
	UPM_ERROR_OUT_OF_RANGE,				/* When the input to drive is too high/low or -ve */
	UPM_ERROR_OPERATION_FAILED,			/* When a function isn't able to perform as expected */
	UPM_ERROR_TIMED_OUT,
	UPM_ERROR_INVALID_PACKET_SIZE
} upm_result_t;

/* Sensor categories */
typedef enum {
    UPM_ACCELEROMETER,
    UPM_COMPASS,
    UPM_DISTANCE,
    UPM_GYROSCOPE,
    UPM_HEART_RATE,
    UPM_IMU,
    UPM_LIGHT,
    UPM_PH,
    UPM_PRESSURE,
    UPM_SERVO,
    UPM_STEPPER,
    UPM_TEMPERATURE,
    UPM_TOUCH
} upm_sensor_t;

/* Supported IO protocols via MRAA */
typedef enum {
	UPM_ANALOG,
	UPM_GPIO,
	UPM_PWM,
	UPM_I2C,
	UPM_SPI,
	UPM_UART,
    UPM_ONEWIRE
} upm_protocol_t;

/* Sensor descriptor */
typedef struct _upm_sensor_descriptor {
    char* name;
    char* description;
    int category_size;
    upm_sensor_t* category;
} upm_sensor_descriptor_t;

#include <types/upm_distance.h>
#include <types/upm_heart_rate.h>
#include <types/upm_ph.h>
#include <types/upm_servo.h>
#include <types/upm_temperature.h>
#include <types/upm_touch.h>

#endif /* UPM_TYPES_H_ */
