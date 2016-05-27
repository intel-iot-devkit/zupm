/*
 * ad8232.h
 *
 *  Created on: May 25, 2016
 *      Author: malikabh
 */

#ifndef AD8232_H_
#define AD8232_H_

#pragma once
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <mraa/aio.h>
#include <mraa/gpio.h>
#include "upm_heart_rate.h"

#define AD8232_DEFAULT_AREF  3.3

typedef struct _upm_ad8232* upm_ad8232;

void* upm_ad8232_init(int num,...);

upm_result_t upm_ad8232_close(void* dev);

upm_result_t upm_ad8232_get_value(void* dev, float* value, upm_heart_rate_units rate_unit);

#endif /* AD8232_H_ */
