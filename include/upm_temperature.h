/*
 * upm_temperature.h
 *
 *  Created on: May 18, 2016
 *      Author: malikabh
 */

#ifndef UPM_TEMPERATURE_H_
#define UPM_TEMPERATURE_H_
#include <stdarg.h>
#include "upm_utilities.h"

typedef enum upm_temp_units {RAW_TEMP, CELSIUS, FAHRENHEIT, KELVIN} upm_temperature_units;
//typedef struct _upm_temp_ft* upm_temperature_func_table;

struct _upm_temp_ft {
	void* (*upm_temperature_init) (int num,...);
	upm_result_t (*upm_temperature_close) (void* dev);
	upm_result_t (*upm_temperature_get_temperature) (void* dev, float* temp, upm_temperature_units temp_unit);
} upm_temperature_func_table;

#endif /* UPM_TEMPERATURE_H_ */
