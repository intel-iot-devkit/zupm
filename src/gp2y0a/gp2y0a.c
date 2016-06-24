/*
 * Author: Jon Trulson <jtrulson@ics.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
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
#include "gp2y0a.h"

struct _upm_gp2y0a {
	mraa_aio_context			aio;
	uint8_t						pin;
	float						a_res;
};

#if defined(CONFIG_BOARD_ARDUINO_101) || defined(CONFIG_BOARD_ARDUINO_101_SSS) || defined(CONFIG_BOARD_QUARK_D2000_CRB)
DEFINE_MEM_MAP(UPM_GP2Y0A_MAP, 1, sizeof(struct _upm_gp2y0a));
const kmemory_map_t UPM_GP2Y0A_MEM_MAP;
#elif defined(linux)
#define UPM_GP2Y0A_MEM_MAP 0
#endif

const char upm_gp2y0a_name[] = "GP2Y0A";
const char upm_gp2y0a_description[] = "Grove 80cm IR Proximity Sensor";
const upm_protocol_t upm_gp2y0a_protocol[] = {UPM_ANALOG};
const upm_sensor_t upm_gp2y0a_category[] = {UPM_DISTANCE};

upm_sensor_descriptor_t upm_gp2y0a_get_descriptor(void* dev){
	upm_sensor_descriptor_t usd;
	usd.name = upm_gp2y0a_name;
	usd.description = upm_gp2y0a_description;
	usd.protocol_size = 1;
	usd.protocol = upm_gp2y0a_protocol;
	usd.category_size = 1;
	usd.category = upm_gp2y0a_category;
	return usd;
}

void* upm_gp2y0a_init_name(int num,...){
	upm_gp2y0a dev = (upm_gp2y0a) upm_malloc(UPM_GP2Y0A_MEM_MAP, sizeof(struct _upm_gp2y0a));

	va_list pin_list;
	va_start(pin_list, num);
	dev->pin = va_arg(pin_list, int);
	dev->a_res = va_arg(pin_list, int);
	dev->aio = mraa_aio_init(dev->pin);
	if(dev->aio == NULL){
		return NULL;
	}
	return dev;
}

void* upm_gp2y0a_init(uint8_t pin, float a_ref){
	upm_gp2y0a dev = (upm_gp2y0a) upm_malloc(UPM_GP2Y0A_MEM_MAP, sizeof(struct _upm_gp2y0a));

	dev->pin = pin;
	dev->a_res = a_ref;
	dev->aio = mraa_aio_init(dev->pin);
	if(dev->aio == NULL){
		return NULL;
	}
	return dev;
}

void upm_gp2y0a_close(void* dev){
	upm_gp2y0a device = (upm_gp2y0a) dev;
	upm_free(UPM_GP2Y0A_MEM_MAP, device);
}

upm_result_t upm_gp2y0a_write(void* dev, void* value, int len){
	return UPM_ERROR_NOT_IMPLEMENTED;
}

upm_result_t upm_gp2y0a_read(void* dev, void* value, int* len){
	upm_gp2y0a device = (upm_gp2y0a) dev;
	int *val = value;
	*val = mraa_aio_read(device->aio);
	return UPM_SUCCESS;
}

upm_result_t upm_gp2y0a_get_value(void* dev, float a_ref, uint8_t samples, float* value){
	upm_gp2y0a device = (upm_gp2y0a) dev;
	int val;
	int sum = 0;
	int i = 0;
	int len;
	for(i=0; i<samples; i++){
		//val = mraa_aio_read(device->aio);
		upm_gp2y0a_read(device, &val, &len);
		sum += val;
	}
	val = sum/samples;
	float volts = (float)(val * a_ref) / (float)device->a_res;
	*value = volts;
	return UPM_SUCCESS;
}
