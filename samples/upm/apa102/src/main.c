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
