/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Contributions: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Contributions: Sarah Knepper <sarah.knepper@intel.com>
 * 				  Abhishek Malik <abhishek.malik@intel.com>
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
#include "led.h"

struct _upm_led {
	mraa_gpio_context		gpio;
	uint8_t					led_pin;
};

#if defined(CONFIG_BOARD_ARDUINO_101) || defined(CONFIG_BOARD_ARDUINO_101_SSS) || defined(CONFIG_BOARD_QUARK_D2000_CRB)
DEFINE_MEM_MAP(UPM_LED_MEM_MAP, 1, sizeof(struct _upm_led));
const kmemory_map_t UPM_LED_MEM_MAP;
#elif defined(linux)
#define UPM_LED_MEM_MAP 0
#endif

const char upm_led_name[] = "LED";
const char upm_led_description[] = "LED";
const upm_protocol_t upm_led_protocol[] = {UPM_GPIO};
const upm_sensor_t upm_led_category[] = {UPM_LED};

void* upm_led_get_ft(upm_sensor_t sensor_type){
	if(sensor_type == UPM_SENSOR){
		struct _upm_sensor_ft *ft = malloc(sizeof(*ft));
		if(ft == NULL){
			printf("Unable to assign memory to the LED structure");
			return NULL;
		}
		ft->upm_sensor_init_name = upm_led_init_name;
		ft->upm_sensor_close = upm_led_close;
		ft->upm_sensor_read = upm_led_read;
		ft->upm_sensor_write = upm_led_write;
		return ft;
	}
	return NULL;
}

upm_sensor_descriptor_t upm_led_get_descriptor(void* dev){
	upm_sensor_descriptor_t usd;
	usd.name = upm_led_name;
	usd.description = upm_led_description;
	usd.protocol_size = 1;
	usd.protocol = upm_led_protocol;
	usd.category_size = 1;
	usd.category = upm_led_category;
	return usd;
}

void* upm_led_init_name(int num,...){
	return NULL;
}

void* upm_led_init(uint8_t pin){
	upm_led dev = (upm_led) upm_malloc(UPM_LED_MEM_MAP, sizeof(struct _upm_led));
	dev->led_pin = pin;
	dev->gpio = mraa_gpio_init(dev->led_pin);
	if(mraa_gpio_dir(dev->gpio, MRAA_GPIO_OUT) != MRAA_SUCCESS)
		return NULL;
	return dev;
}

void upm_led_close(void* dev){
	upm_free(UPM_LED_MEM_MAP, dev);
}

upm_result_t upm_led_write(void* dev, void* value, int len){
	upm_led device = (upm_led) dev;
	int* int_val = value;
	mraa_result_t ret;
	if(*int_val >= 1)
		ret = mraa_gpio_write(device->gpio, 1);
	else
		ret = mraa_gpio_write(device->gpio, 0);
	if(ret != MRAA_SUCCESS)
		return UPM_ERROR_OPERATION_FAILED;
	return UPM_SUCCESS;
}

upm_result_t upm_led_read(void* dev, void* value, int* len){
	return UPM_ERROR_NOT_IMPLEMENTED;
}

upm_result_t upm_led_on(void* dev){
	upm_led device = (upm_led) dev;
	int len = 0, val = 1;
	if(upm_led_write(device, &val, len) != UPM_SUCCESS)
		return UPM_ERROR_OPERATION_FAILED;
	return UPM_SUCCESS;
}

upm_result_t upm_led_off(void* dev){
	upm_led device = (upm_led) dev;
	int len = 0, val = 0;
	if(upm_led_write(device, &val, len) != UPM_SUCCESS)
		return UPM_ERROR_OPERATION_FAILED;
	return UPM_SUCCESS;
}
