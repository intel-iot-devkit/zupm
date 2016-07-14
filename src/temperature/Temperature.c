/*
 * Author: Sisinty Sasmita Patra <sisinty.s.patra@intel.com>
 *
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


#include<stdio.h>
#include <stdlib.h>

#include "temperature.h"
#include "upm_sensor.h"
#include "upm.h"

struct _upm_temperature{
    mraa_aio_context aio;
    int16_t m_aRes;
};

const char upm_temperature_name[] = "Grove Temperature";
const char upm_temperature_description[] = "Analog Grove Temperature Sensor";
const upm_protocol_t upm_temperature_protocol[] = {UPM_ANALOG};
const upm_protocol_t upm_temperature_category[] = {UPM_TEMPERATURE};

const upm_sensor_descriptor_t upm_temperature_get_descriptor(void* dev) {
    upm_sensor_descriptor_t usd;
    usd.name = upm_temperature_name;
    usd.description = upm_temperature_description;
    usd.protocol_size = 1;
    usd.protocol = upm_temperature_protocol;
    usd.category_size = 1;
    usd.category = upm_temperature_category;
    return usd;
}

const void* (*upm_get_ft) (upm_sensor_t sensor_type) = &upm_temperature_get_ft;

void* upm_temperature_get_ft(upm_sensor_t sensor_type) {

    if(sensor_type == UPM_SENSOR) {
        upm_sensor_ft *ft = malloc(sizeof(*ft));
        //ft->upm_sensor_init_name = upm_temperature_init_name;
        ft->upm_sensor_close = upm_temperature_close;
        ft->upm_sensor_read = upm_temperature_read;
        ft->upm_sensor_write = upm_temperature_write;
        return ft;
    }

    if(sensor_type == UPM_TEMPERATURE) {
        struct _upm_temperature_ft *tft = malloc(sizeof(*tft));
        if(tft == NULL){
            printf("Unable to assign memory");
            return NULL;
        }
        tft->upm_temperature_get_value = upm_temperature_get_value_temperature;
        return tft;
    }
    return NULL;
}

//void* upm_temperature_init_name(...);

void* upm_temperature_init(int pin)
{
    upm_temperature dev = (upm_temperature) malloc(sizeof(struct _upm_temperature));

    if(dev == NULL) return NULL;

    dev->aio = mraa_aio_init(pin);

    dev->m_aRes = (1 << mraa_aio_get_bit(dev->aio));

    if(dev->aio == NULL)
    {
        free(dev);
        return NULL;
    }
    return dev;
}

void upm_temperature_close(void* dev)
{
    upm_temperature device = (upm_temperature) dev;
    mraa_aio_close(device->aio);
    free(dev);
}

upm_result_t upm_temperature_read (void* dev, void* value, int len)
{
    upm_temperature device = (upm_temperature) dev;

    *(float*)value = mraa_aio_read(device->aio);

    return UPM_SUCCESS;
}

upm_result_t upm_temperature_write(void* dev, void* value, int len)
{
    return  UPM_ERROR_NOT_SUPPORTED;
}

upm_result_t upm_temperature_get_value_temperature (void* dev, float* tempval, upm_temperature_u unit)
{
    upm_temperature device = (upm_temperature) dev;

    float val = 0.0;
    float res = 0.0;

    upm_temperature_read(device, &val, 0);

    //resistance = res
    res = (device->m_aRes - val) * 10000.0f/val;

    if (unit == KELVIN){
        *tempval = 1.0f/(log(res/10000.0f)/3975.0f + 1.0f/298.15f);
    }
    else if(unit == CELSIUS) {
        *tempval = 1.0f/(log(res/10000.0f)/3975.0f + 1.0f/298.15f)-273.15f;
    }
    else if (unit == FAHRENHEIT){
        *tempval = (1.0f/(log(res/10000.0f)/3975.0f + 1.0f/298.15f)-273.15f) * 1.8f + 32.0f;
    }

    return UPM_SUCCESS;
}
