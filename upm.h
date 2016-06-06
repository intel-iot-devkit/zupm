/*
 * upm.h
 *
 *  Created on: Jun 1, 2016
 *      Author: malikabh
 */

#ifndef UPM_H_
#define UPM_H_

#include "upm_utilities.h"

void* upm_sensor_init(int num,...);
upm_result_t upm_sensor_close(void* dev);
upm_result_t upm_sensor_read(void* dev, void* data);
upm_result_t upm_sensor_write(void* dev, void* data, int len);

#endif /* UPM_H_ */
