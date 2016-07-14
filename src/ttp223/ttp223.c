/*
 * Author: Sarah Knepper <sarah.knepper@intel.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2015 Intel Corporation.
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

#include "ttp223.h"

struct _upm_ttp223 {
    mraa_gpio_context       gpio;
    uint8_t                 gpio_pin;
    bool                    isr_installed;
};

#if defined(CONFIG_BOARD_ARDUINO_101) || defined(CONFIG_BOARD_ARDUINO_101_SSS) || defined(CONFIG_BOARD_QUARK_D2000_CRB)
DEFINE_MEM_MAP(UPM_TTP223_MEM_MAP, 1, sizeof(struct _upm_ttp223));
const kmemory_map_t UPM_TTP223_MEM_MAP;
#elif defined(linux)
#define UPM_TTP223_MEM_MAP 0
#endif

const char upm_ttp223_name[] = "TTP223";
const char upm_ttp223_description[] = "TTP223 Touch Detector Sensor";
const upm_protocol_t upm_ttp223_protocol[] = {UPM_GPIO};
const upm_sensor_t upm_ttp223_category[] = {UPM_SWITCH};

const upm_sensor_descriptor_t upm_ttp223_get_descriptor(){
    upm_sensor_descriptor_t usd;
    usd.name = upm_ttp223_name;
    usd.description = upm_ttp223_description;
    usd.protocol_size = 1;
    usd.protocol = upm_ttp223_protocol;
    usd.category_size = 1;
    usd.category = upm_ttp223_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_ttp223_init_name,
    .upm_sensor_close = &upm_ttp223_close,
    .upm_sensor_get_descriptor = &upm_ttp223_get_descriptor
};

static const upm_switch_ft sft =
{
    .upm_switch_get_value = &upm_ttp223_is_pressed
};

const void* (*upm_get_ft) (upm_sensor_t sensor_type) = &upm_ttp223_get_ft;

const void* upm_ttp223_get_ft(upm_sensor_t sensor_type){
    if(sensor_type == UPM_SWITCH){
        return &sft;
    }
    else if(sensor_type == UPM_SENSOR){
        return &ft;
    }
    return NULL;
}

void* upm_ttp223_init_name(){
    return NULL;
}

void* upm_ttp223_init(int pin){
    upm_ttp223 dev = (upm_ttp223) malloc(sizeof(struct _upm_ttp223));
    if(dev == NULL){
        printf("Unable to allocate space for the sensor struct\n");
        return NULL;
    }

    dev->gpio_pin = pin;

    dev->gpio = mraa_gpio_init(dev->gpio_pin);
    mraa_gpio_dir(dev->gpio, MRAA_GPIO_IN);
    dev->isr_installed = false;
    return dev;
}

void upm_ttp223_close(void* dev){
//        upm_free(UPM_TTP223_MEM_MAP, dev);
}

upm_result_t upm_ttp223_get_value(mraa_gpio_context dev, uint32_t* value){

    *value = mraa_gpio_read(dev);

    return UPM_SUCCESS;
}

upm_result_t upm_ttp223_is_pressed(void* dev, bool* value, int num){
    upm_ttp223 device = (upm_ttp223) dev;
    if(num != 1){
        printf("Sorry this sensor driver supports only one touch pad\n");
        return UPM_ERROR_OUT_OF_RANGE;
    }
    uint32_t ret = false;
    upm_ttp223_get_value(device->gpio, &ret);
    if(ret == 1)
        *value = true;
    else
        *value = false;
    return UPM_SUCCESS;
}

upm_result_t upm_ttp223_install_isr(void* dev, mraa_gpio_edge_t edge_level, void (*isr)(void *), void *arg){
    upm_ttp223 device = (upm_ttp223) dev;
    if(device->isr_installed)
        upm_ttp223_uninstall_isr(device);

    mraa_gpio_isr(device->gpio, edge_level, isr, arg);
    device->isr_installed = true;
    return UPM_SUCCESS;
}

upm_result_t upm_ttp223_uninstall_isr(void* dev){
    upm_ttp223 device = (upm_ttp223) dev;
    mraa_gpio_isr_exit(device->gpio);
    device->isr_installed = false;
    return UPM_SUCCESS;
}
