/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * 		   Abhishek Malik <abhishek.malik@intel.com>
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
#include "mq303a.h"

struct _upm_mq303a {
	mraa_gpio_context		gpio;
	mraa_aio_context		aio;
	uint8_t					gpio_pin;
	uint8_t					aio_pin;
};

#if defined(CONFIG_BOARD_ARDUINO_101) || defined(CONFIG_BOARD_ARDUINO_101_SSS) || defined(CONFIG_BOARD_QUARK_D2000_CRB)
DEFINE_MEM_MAP(UPM_MQ303A_MEM_MAP, 1, sizeof(struct _upm_mq303a));
const kmemory_map_t UPM_MQ303A_MEM_MAP;
#elif defined(linux)
#define UPM_MQ303A_MEM_MAP 0
#endif

const char upm_mq303a_name[] = "MQ303A";
const char upm_mq303a_description[] = "MQ303A";
const upm_protocol_t upm_mq303a_protocol[] = {UPM_GPIO, UPM_ANALOG};
const upm_sensor_t upm_mq303a_category[] = {UPM_GAS};

upm_sensor_descriptor_t upm_mq303a_get_descriptor(void* dev){
	upm_sensor_descriptor_t usd;
	usd.name = upm_mq303a_name;
	usd.description = upm_mq303a_description;
	usd.protocol_size = 1;
	usd.protocol = upm_mq303a_protocol;
	usd.category_size = 1;
	usd.category = upm_mq303a_category;
	return usd;
}

void* upm_mq303a_get_ft(upm_sensor_t sensor_type){
	if(sensor_type == UPM_SENSOR){
		struct _upm_sensor_ft *ft = malloc(sizeof(*ft));
		if(ft == NULL){
			printf("Unable to assign memory to the LED structure");
			return NULL;
		}
		ft->upm_sensor_init_name = upm_mq303a_init_name;
		ft->upm_sensor_close = upm_mq303a_close;
		ft->upm_sensor_read = upm_mq303a_read;
		ft->upm_sensor_write = upm_mq303a_write;
		return ft;
	}
	return NULL;
}

void* upm_mq303a_init_name(int num,...){
	return NULL;
}

void* upm_mq303a_init(int pin, int heater_pin){
	upm_mq303a dev = (upm_mq303a) upm_malloc(UPM_MQ303A_MEM_MAP, sizeof(struct _upm_mq303a));

	dev->aio_pin = pin;
	dev->gpio_pin = heater_pin;
	dev->aio = mraa_aio_init(dev->aio_pin);
	dev->gpio = mraa_gpio_init(dev->gpio_pin);
	if(mraa_gpio_dir(dev->gpio, MRAA_GPIO_OUT) != MRAA_SUCCESS)
		return NULL;
	return dev;
}

void upm_mq303a_close(void* dev){
	upm_free(UPM_MQ303A_MEM_MAP, dev);
}

upm_result_t upm_mq303a_heater_enable(void* dev, bool enable){
	upm_mq303a device = (upm_mq303a) dev;
	if(enable)
		mraa_gpio_write(device->gpio, 0);
	else
		mraa_gpio_write(device->gpio, 1);
	return UPM_SUCCESS;
}

upm_result_t upm_mq303a_read(void* dev, void* value, int* len){
	upm_mq303a device = (upm_mq303a) dev;
	int* int_val = value;
	*int_val = mraa_aio_read(device->aio);
	return UPM_SUCCESS;
}

upm_result_t upm_mq303a_write(void* dev, void* value, int len){
	return UPM_ERROR_NOT_IMPLEMENTED;
}

upm_result_t upm_mq303a_get_value(void* dev, int* val){
	upm_mq303a device = (upm_mq303a) dev;
	int len = 0;
	return upm_mq303a_read(device, val, &len);
}
