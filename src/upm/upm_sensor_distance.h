/*
 * upm_sensor_distance.h
 *
 *  Created on: May 18, 2016
 *      Author: malikabh
 */

#ifndef UPM_SENSOR_DISTANCE_H_
#define UPM_SENSOR_DISTANCE_H_
#include <stdarg.h>
#include "upm_utilities.h"

typedef enum upm_dist_units {RAW_DIST, CENTIMETER, INCH} upm_distance_units;
//typedef struct _upm_dist_ft* upm_distance_func_table;

struct _upm_dist_ft {
	void* (*upm_distance_init) (int num,...);
	upm_result_t (*upm_distance_close) (void* dev);
	upm_result_t (*upm_distance_get_distance) (void* dev, float* dist, upm_distance_units dist_unit);
} upm_distance_func_table;

#endif /* UPM_SENSOR_DISTANCE_H_ */
