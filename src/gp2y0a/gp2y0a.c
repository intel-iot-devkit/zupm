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
    mraa_aio_context            aio;
    uint8_t                     pin;
    int                         a_res;
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

const upm_sensor_descriptor_t upm_gp2y0a_get_descriptor(){
    upm_sensor_descriptor_t usd;
    usd.name = upm_gp2y0a_name;
    usd.description = upm_gp2y0a_description;
    usd.protocol_size = 1;
    usd.protocol = upm_gp2y0a_protocol;
    usd.category_size = 1;
    usd.category = upm_gp2y0a_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_gp2y0a_init_name,
    .upm_sensor_close = &upm_gp2y0a_close,
    .upm_sensor_get_descriptor = &upm_gp2y0a_get_descriptor
};

#if defined(FRAMEWORK_BUILD)
typedef const void* (*upm_get_ft) (upm_sensor_t sensor_type);

upm_get_ft upm_assign_ft(){
    return upm_gp2y0a_get_ft;
}
#endif

const void* upm_gp2y0a_get_ft(upm_sensor_t sensor_type)
{
    switch(sensor_type)
    {
        case UPM_SENSOR:
            return &ft;
        default:
            return NULL;
    }
}

void* upm_gp2y0a_init_name(){
    return NULL;
}

void* upm_gp2y0a_init(uint8_t pin, float a_ref){
    upm_gp2y0a dev = (upm_gp2y0a) upm_malloc(UPM_GP2Y0A_MEM_MAP, sizeof(struct _upm_gp2y0a));

    dev->pin = pin;
    dev->aio = mraa_aio_init(dev->pin);
    dev->a_res = (1 << mraa_aio_get_bit(dev->aio));
    if(dev->aio == NULL){
        return NULL;
    }
    return dev;
}

void upm_gp2y0a_close(void* dev){
    upm_gp2y0a device = (upm_gp2y0a) dev;
    upm_free(UPM_GP2Y0A_MEM_MAP, device);
}

upm_result_t upm_gp2y0a_get_value(void* dev, float a_ref,
                                  uint8_t samples, float* value){
    upm_gp2y0a device = (upm_gp2y0a) dev;
    int val;
    int sum = 0;
    int i = 0;
    int len;

    for(i=0; i<samples; i++) {
        val = mraa_aio_read(device->aio);
        sum += val;
    }
    val = sum/samples;
    float volts = (float)(val * a_ref) / (float)device->a_res;
    *value = volts;

    return UPM_SUCCESS;
}
