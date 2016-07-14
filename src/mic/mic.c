/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * 	   Sisinty Sasmita Patra <sisinty.s.patra@intel.com>
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

#include "mic.h"

struct _upm_mic{
    mraa_aio_context aio;
    uint16_t analog_pin;
};

const char upm_mic_name[] = "Microphone";
const char upm_mic_description[] = "Analog Microphone";
const upm_protocol_t upm_mic_protocol[] = {UPM_ANALOG};
const upm_protocol_t upm_mic_category[] = {UPM_AUDIO};

const upm_sensor_descriptor_t upm_mic_get_descriptor(void* dev) {
    upm_sensor_descriptor_t usd;
    usd.name = upm_mic_name;
    usd.description = upm_mic_description;
    usd.protocol_size = 1;
    usd.protocol = upm_mic_protocol;
    usd.category_size = 1;
    usd.category = upm_mic_category;
    return usd;
}

const void* (*upm_get_ft) (upm_sensor_t sensor_type) = &upm_mic_get_ft;

void* upm_mic_get_ft(upm_sensor_t sensor_type) {

    if(sensor_type == UPM_SENSOR) {
        upm_sensor_ft *ft = malloc(sizeof(upm_sensor_ft));
        if(ft == NULL){
            printf("Unable to assign memory");
            return NULL;
        }

        //ft->upm_sensor_init_name = upm_mic_init_name;
        ft->upm_sensor_close = upm_mic_close;
        ft->upm_sensor_read = upm_mic_read;
        ft->upm_sensor_write = upm_mic_write;
        return ft;
    }

    if(sensor_type == UPM_AUDIO) {
        struct _upm_audio_ft *sft = malloc(sizeof(*sft));
        if(sft == NULL){
            printf("Unable to assign memory");
            return NULL;
        }

        sft->upm_audio_get_value = upm_mic_get_value;
        return sft;
    }
    return NULL;
}

void* upm_mic_init(int pin)
{
    upm_mic dev = (upm_mic) malloc(sizeof(struct _upm_mic));

    if(dev == NULL) return NULL;

    dev->analog_pin = pin;
    dev->aio = mraa_aio_init(dev->analog_pin);

    if(dev->aio == NULL)
    {
        printf("unable to initialize the AIO pin");
        return NULL;
    }
    return dev;
}

//void* upm_mic_init_name(char* protocol, char* params);

void upm_mic_close(void* dev)
{
    upm_mic device = (upm_mic) dev;
    mraa_aio_close(device->aio);
    free(dev);
}

/* gives the raw value */
upm_result_t upm_mic_read(void* dev, void *val, int len)
{
    upm_mic device = (upm_mic) dev;

    *(float*)val = mraa_aio_read(device->aio);

    return UPM_SUCCESS;
}

upm_result_t upm_mic_write(void* dev, void *value, int len)
{
    return  UPM_ERROR_NOT_SUPPORTED;
}

upm_result_t upm_mic_get_value(void* dev, float *micval, upm_audio_u unit)
{
    upm_mic device = (upm_mic) dev;
    upm_mic_read(device, micval, 0);

    //convert readings to decibels
    if(unit == DECIBELS) {
    }
    return UPM_SUCCESS;
}
