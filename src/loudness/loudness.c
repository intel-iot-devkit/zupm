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
#include "loudness.h"

struct _upm_loudness {
    mraa_aio_context            aio;
    uint8_t                     pin;
};

#if defined(CONFIG_BOARD_ARDUINO_101) || defined(CONFIG_BOARD_ARDUINO_101_SSS) || defined(CONFIG_BOARD_QUARK_D2000_CRB)
DEFINE_MEM_MAP(UPM_LOUDNESS_MAP, 1, sizeof(struct _upm_loudness));
const kmemory_map_t UPM_LOUDNESS_MEM_MAP;
#elif defined(linux)
#define UPM_LOUDNESS_MEM_MAP 0
#endif

const char upm_loudness_name[] = "loudness";
const char upm_loudness_description[] = " Loudness Sensor";
const upm_protocol_t upm_loudness_protocol[] = {UPM_ANALOG};
const upm_sensor_t upm_loudness_category[] = {UPM_AUDIO};

const upm_sensor_descriptor_t upm_loudness_get_descriptor(){
    upm_sensor_descriptor_t usd;
    usd.name = upm_loudness_name;
    usd.description = upm_loudness_description;
    usd.protocol_size = 1;
    usd.protocol = upm_loudness_protocol;
    usd.category_size = 1;
    usd.category = upm_loudness_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_loudness_init_name,
    .upm_sensor_close = &upm_loudness_close,
    .upm_sensor_read = &upm_loudness_read,
    .upm_sensor_write = &upm_loudness_write,
    .upm_sensor_get_descriptor = &upm_loudness_get_descriptor
};

const void* (*upm_get_ft) (upm_sensor_t sensor_type) = &upm_loudness_get_ft;

const void* upm_loudness_get_ft(upm_sensor_t sensor_type){
    if(sensor_type == UPM_SENSOR){
        return &ft;
    }
    return NULL;
}

void* upm_loudness_init_name(){
    return NULL;
}

void* upm_loudness_init(int pin){
    upm_loudness dev = (upm_loudness) upm_malloc(UPM_LOUDNESS_MEM_MAP, sizeof(struct _upm_loudness));

    dev->pin = pin;
    dev->aio = mraa_aio_init(dev->pin);

    return dev;
}

void upm_loudness_close(void* dev){
    upm_loudness device = (upm_loudness) dev;
    mraa_aio_close(device->aio);
    upm_free(UPM_LOUDNESS_MEM_MAP, dev);
}

upm_result_t upm_loudness_read(const void* dev, void* value, int len){
    upm_loudness device = (upm_loudness) dev;
    int *val = value;
    *val = mraa_aio_read(device->aio);
    return UPM_SUCCESS;
}

upm_result_t upm_loudness_write(const void* dev, void* value, int len){
    return UPM_ERROR_NOT_IMPLEMENTED;
}

upm_result_t upm_loudness_get_value(void* dev, int* val){
    upm_loudness device = (upm_loudness) dev;
    int len;
    upm_loudness_read(device, val, len);
    return UPM_SUCCESS;
}
