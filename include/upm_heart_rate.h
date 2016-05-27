/*
 * upm_heart_rate.h
 *
 *  Created on: May 25, 2016
 *      Author: malikabh
 */

#ifndef UPM_HEART_RATE_H_
#define UPM_HEART_RATE_H_

#include <stdarg.h>
#include "upm_utilities.h"

typedef enum _upm_heart_rate_units {RAW_HR, HEART_RATE} upm_heart_rate_units;

struct _upm_hr_ft {
	void* (*upm_heart_rate_sensor_init) (int num,...);
	upm_result_t (*upm_heart_rate_sensor_close) (void* dev);
	upm_result_t (*upm_heart_rate_sensor_get_heart_rate) (void* dev, float* dist, upm_heart_rate_units rate_unit);
} upm_hr_func_table;

#endif /* UPM_HEART_RATE_H_ */
