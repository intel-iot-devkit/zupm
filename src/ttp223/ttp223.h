/*
 * ttp223.h
 *
 *  Created on: May 26, 2016
 *      Author: malikabh
 */

#ifndef UPM_C_TTP223_TTP223_H_
#define UPM_C_TTP223_TTP223_H_

#pragma once
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <mraa/gpio.h>
#include "upm_sensor_touch.h"

typedef struct _upm_ttp223* upm_ttp223;

void* upm_ttp223_init(int num,...);

upm_result_t upm_ttp223_close(void* dev);

upm_result_t upm_ttp223_is_pressed(void* dev, upm_boolean_t* value, int num);

upm_result_t upm_ttp223_install_isr(void* dev, mraa_gpio_edge_t edge_level, void (*isr)(void *), void *arg);

upm_result_t upm_ttp223_uninstall_isr(void* dev);

#endif /* UPM_C_TTP223_TTP223_H_ */
