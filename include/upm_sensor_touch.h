/*
 * upm_sensor_touch.h
 *
 *  Created on: May 26, 2016
 *      Author: malikabh
 */

#ifndef UPM_SENSOR_TOUCH_H_
#define UPM_SENSOR_TOUCH_H_
#include <stdarg.h>
#include "upm_utilities.h"

struct _upm_touch_ft {
	void* (*upm_touch_init) (int num,...);
	upm_result_t (*upm_touch_close) (void* dev);
	upm_result_t (*upm_touch_get_is_pressed) (void* dev, upm_boolean_t* value, int num);
} upm_touch_func_table;

#endif /* UPM_SENSOR_TOUCH_H_ */
