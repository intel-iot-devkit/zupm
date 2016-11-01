/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <zephyr.h>
#include <apa102.h>
#include <spi.h>
#include <malloc.h>
#include <pinmux.h>
#include <sys_clock.h>
#include "upm_utilities.h"

#define SLEEPTICKS	SECONDS(1)
#if defined(CONFIG_STDOUT_CONSOLE)
#include <stdio.h>
#define PRINT           printf
#else
#include <misc/printk.h>
#define PRINT           printk
#endif

/*
 * @file
 * @brief Hello World demo
 * Nanokernel version of hello world demo
 */


void main(void)
{
	printf("starting program\n");
	mraa_init();
	struct nano_timer timer;
	void *timer_data[1];
	nano_timer_init(&timer, timer_data);
	apa102_context dev = apa102_init(60, 0, -1);

// setting to red
	if(apa102_set_leds(dev, 0, 29, 5, 255, 0, 0) != UPM_SUCCESS){
		printf("error while setting the leds\n");
	}
	if(apa102_refresh(dev) != UPM_SUCCESS){
		printf("error while dumping the buffer onto the LEDs\n");
	}
upm_delay(1);
// setting to green
	if(apa102_set_leds(dev, 0, 29, 5, 0, 255, 0) != UPM_SUCCESS){
		printf("error while setting the leds\n");
	}
	if(apa102_refresh(dev) != UPM_SUCCESS){
		printf("error while dumping the buffer onto the LEDs\n");
	}
upm_delay(1);
// setting to blue
	if(apa102_set_leds(dev, 0, 29, 5, 0, 0, 255) != UPM_SUCCESS){
		printf("error while setting the leds\n");
	}
	if(apa102_refresh(dev) != UPM_SUCCESS){
		printf("error while dumping the buffer onto the LEDs\n");
	}
	apa102_close(dev);
}
