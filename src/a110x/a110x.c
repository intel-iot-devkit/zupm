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
#include "a110x.h"

struct _upm_a110x {
    mraa_gpio_context      gpio;
    uint8_t                gpio_pin;
    bool                   isr_installed;
};

#if defined(CONFIG_BOARD_ARDUINO_101) || defined(CONFIG_BOARD_ARDUINO_101_SSS) || defined(CONFIG_BOARD_QUARK_D2000_CRB)
DEFINE_MEM_MAP(UPM_A110X_MEM_MAP, 1, sizeof(struct _upm_a110x));
const kmemory_map_t UPM_A110X_MEM_MAP;
#elif defined(linux)
#define UPM_A110X_MEM_MAP 0
#endif

const char upm_a110x_name[] = "A110X";
const char upm_a110x_description[] = "A110X Hall Effect Sensor";
const upm_protocol_t upm_a110x_protocol[] = {UPM_GPIO};
const upm_sensor_t upm_a110x_category[] = {UPM_ELECTRICITY};

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_a110x_init_name,
    .upm_sensor_close = &upm_a110x_close,
    .upm_sensor_get_descriptor = &upm_a110x_get_descriptor
};

const void* (*upm_get_ft) (upm_sensor_t sensor_type) = &upm_a110x_get_ft;

const void* upm_a110x_get_ft(upm_sensor_t sensor_type){
    if(sensor_type == UPM_SENSOR){
        return &ft;
    }
    return NULL;
}

const upm_sensor_descriptor_t upm_a110x_get_descriptor(){
    upm_sensor_descriptor_t usd;
    usd.name = upm_a110x_name;
    usd.description = upm_a110x_description;
    usd.protocol_size = 1;
    usd.protocol = upm_a110x_protocol;
    usd.category_size = 1;
    usd.category = upm_a110x_category;
    return usd;
}

void* upm_a110x_init_name(){
    return NULL;
}

void* upm_a110x_init(uint8_t pin){
    upm_a110x dev = (upm_a110x) upm_malloc(UPM_A110X_MEM_MAP, sizeof(struct _upm_a110x));
    dev->gpio_pin = pin;
    dev->gpio = mraa_gpio_init(dev->gpio_pin);
    if(mraa_gpio_dir(dev->gpio, MRAA_GPIO_IN) != MRAA_SUCCESS){
        return NULL;
    }
    dev->isr_installed = false;
    return dev;
}

void upm_a110x_close(void* dev){
    upm_free(UPM_A110X_MEM_MAP, dev);
}

upm_result_t upm_a110x_magnet_detected(void* dev, bool* res){
    upm_a110x device = (upm_a110x) dev;

    int val = mraa_gpio_read(device->gpio);
    if(val == 0)
        *res = false;
    else
        *res = true;
    return UPM_SUCCESS;
}

upm_result_t upm_a110x_install_isr(void* dev, mraa_gpio_edge_t edge_level,
                                   void (*isr)(void *), void *arg){
    upm_a110x device = (upm_a110x) dev;
    if(device->isr_installed)
        upm_a110x_uninstall_isr(device);

    mraa_gpio_isr(device->gpio, edge_level, isr, arg);
    device->isr_installed = true;
    return UPM_SUCCESS;
}

upm_result_t upm_a110x_uninstall_isr(void* dev){
    upm_a110x device = (upm_a110x) dev;
    mraa_gpio_isr_exit(device->gpio);
    device->isr_installed = false;
    return UPM_SUCCESS;
}
