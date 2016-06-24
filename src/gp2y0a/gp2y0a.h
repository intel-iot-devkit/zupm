/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * 		   Abhishek Malik <abhishek.malik@intel.com>
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

#ifndef GP2Y0A_GP2Y0A_H_
#define GP2Y0A_GP2Y0A_H_

#include "../upm.h"

/**
 * @brief GP2Y0A-based IR Proximity Sensor library
 * @defgroup gp2y0a libupm-gp2y0a
 * @ingroup seeed analog light
 */

/**
 * @library gp2y0a
 * @sensor gp2y0a
 * @comname GP2Y0A IR Proximity Sensor
 * @altname Grove 80cm IR Proximity Sensor
 * @type light
 * @man seeed
 * @con analog
 *
 * @brief API for the GP2Y0A family of IR Proximity Sensors
 *
 * Sensors of this family return an analog voltage corresponding to the distance
 * of an object from the sensor. The voltage is lower when objects
 * are far away; the voltage increases as objects get closer
 * to the sensor.
 *
 * @image html gp2y0a.jpg
 * @snippet gp2y0a.cxx Interesting
 */
typedef struct _upm_gp2y0a* upm_gp2y0a;

void* upm_gp2y0a_init_name(int num,...);

/**
 * GP2Y0A sensor init function
 *
 * @param pin Analog pin to use
 * @param a_ref reference voltage
 */
void* upm_gp2y0a_init(uint8_t pin, float a_ref);

/**
 * GP2Y0A close function
 *
 * @param dev sensor struct
 */
void upm_gp2y0a_close(void* dev);

/**
 * Gets an average voltage value from the sensor
 *
 * @param dev sensor struct
 * @param aref Reference voltage in use (usually 5.0V or 3.3V)
 * @param samples Number of samples to average over
 * @return Average voltage reading
 */
upm_result_t upm_gp2y0a_get_value(void* dev, float a_ref, uint8_t samples, float* val);

/*
 * Generic Read function
 */
upm_result_t upm_gp2y0a_read(void* dev, void* value, int* len);

/*
 * Generic write function
 */
upm_result_t upm_gp2y0a_write(void* dev, void* value, int len);

#endif /* GP2Y0A_GP2Y0A_H_ */
