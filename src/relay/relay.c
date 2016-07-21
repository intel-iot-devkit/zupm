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

#include "relay.h"

struct _upm_relay{
    mraa_gpio_context gpio;
};

const char upm_relay_name[] = "Grove Relay";
const char upm_relay_description[] = "Grove relay Sensor";
const upm_protocol_t upm_relay_protocol[] = {UPM_GPIO};
const upm_sensor_t upm_relay_category[] = {UPM_SWITCH};

const upm_sensor_descriptor_t upm_relay_get_descriptor() {
    upm_sensor_descriptor_t usd;
    usd.name = upm_relay_name;
    usd.description = upm_relay_description;
    usd.protocol_size = 1;
    usd.protocol = upm_relay_protocol;
    usd.category_size = 1;
    usd.category = upm_relay_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_relay_init_name,
    .upm_sensor_close = &upm_relay_close,
    .upm_sensor_get_descriptor = &upm_relay_get_descriptor
};

static const upm_switch_ft sft =
{
    .upm_switch_get_value = &upm_relay_get_value
};

const void* upm_relay_get_ft(upm_sensor_t sensor_type) {
    if(sensor_type == UPM_SENSOR) {
        return &ft;
    }
    if(sensor_type == UPM_SWITCH) {
        return &sft;
    }
    return NULL;
}

void* upm_relay_init_name(){
    return NULL;
}

void* upm_relay_init(int pin)
{
    upm_relay dev = (upm_relay) malloc(sizeof(struct _upm_relay));

    if(dev == NULL) return NULL;

    dev->gpio = mraa_gpio_init(pin);

    if(dev->gpio == NULL)
    {
        free(dev);
        return NULL;
    }
    return dev;
}

void upm_relay_close(void* dev)
{
    upm_relay device = (upm_relay) dev;
    mraa_gpio_close(device->gpio);

    free(dev);
}

upm_result_t upm_relay_on(void* dev)
{
    upm_relay device = (upm_relay) dev;
    //upm_relay_write(device, 1, 0);
    mraa_gpio_write(device->gpio, 1);
    return UPM_SUCCESS;
}

upm_result_t upm_relay_off(void* dev)
{
    upm_relay device = (upm_relay) dev;
    //upm_relay_write(device, 0, 0);
    mraa_gpio_write(device->gpio, 0);
    return UPM_SUCCESS;
}

bool upm_relay_is_on(void* dev)
{
    int val;
    upm_relay device = (upm_relay) dev;
    //upm_relay_read (device, &val, 0);
    val = mraa_gpio_read(device->gpio);
    return UPM_SUCCESS;
}

bool upm_relay_is_off(void* dev)
{
    int val;
    upm_relay device = (upm_relay) dev;
    //upm_relay_read (device, &val, 0);
    val = mraa_gpio_read(device->gpio);
    return UPM_SUCCESS;
}

upm_result_t upm_relay_get_value(void* dev, bool* value, int num)
{
    upm_relay device = (upm_relay) dev;
    //upm_relay_read(device,val,0);
    *value = mraa_gpio_read(device->gpio);
    return UPM_SUCCESS;
}
