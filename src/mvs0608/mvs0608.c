/*
 * Author: Zion Orent <sorent@ics.com>
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
#include "mvs0608.h"

struct _upm_mvs0608 {
    mraa_gpio_context        gpio;
    uint8_t                  gpio_pin;
};

#if defined(CONFIG_BOARD_ARDUINO_101) || defined(CONFIG_BOARD_ARDUINO_101_SSS) || defined(CONFIG_BOARD_QUARK_D2000_CRB)
DEFINE_MEM_MAP(UPM_MVS0608_MEM_MAP, 1, sizeof(struct _upm_mvs0608));
const kmemory_map_t UPM_MVS0608_MEM_MAP;
#elif defined(linux)
#define UPM_MVS0608_MEM_MAP 0
#endif

const char upm_mvs0608_name[] = "MVS0608";
const char upm_mvs0608_description[] = "MVS0608 Grove Collision sensor";
const upm_protocol_t upm_mvs0608_protocol[] = {UPM_GPIO};
const upm_sensor_t upm_mvs0608_category[] = {UPM_ELECTRICITY};

const upm_sensor_descriptor_t upm_mvs0608_get_descriptor(){
    upm_sensor_descriptor_t usd;
    usd.name = upm_mvs0608_name;
    usd.description = upm_mvs0608_description;
    usd.protocol_size = 1;
    usd.protocol = upm_mvs0608_protocol;
    usd.category_size = 1;
    usd.category = upm_mvs0608_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_mvs0608_init_name,
    .upm_sensor_close = &upm_mvs0608_close,
    .upm_sensor_read = &upm_mvs0608_read,
    .upm_sensor_write = &upm_mvs0608_write,
    .upm_sensor_get_descriptor = &upm_mvs0608_get_descriptor
};

const void* upm_mvs0608_get_ft(upm_sensor_t sensor_type){
    if(sensor_type == UPM_SENSOR){
        return &ft;
    }
    return NULL;
}

void* upm_mvs0608_init_name(){
    return NULL;
}

void* upm_mvs0608_init(int pin){
    upm_mvs0608 dev = (upm_mvs0608) upm_malloc(UPM_MVS0608_MEM_MAP, sizeof(struct _upm_mvs0608));
    dev->gpio_pin = pin;
    dev->gpio = mraa_gpio_init(dev->gpio_pin);
    if(mraa_gpio_dir(dev->gpio, MRAA_GPIO_IN) != MRAA_SUCCESS)
        return NULL;
    return dev;
}

void upm_mvs0608_close(void* dev){
    upm_free(UPM_MVS0608_MEM_MAP, dev);
}

upm_result_t upm_mvs0608_read(const void* dev, void* value, int len){
    upm_mvs0608 device = (upm_mvs0608) dev;
    int* int_val = value;
    *int_val = mraa_gpio_read(device->gpio);
    return UPM_SUCCESS;
}

upm_result_t upm_mvs0608_write(const void* dev, void* value, int len){
    return UPM_ERROR_NOT_IMPLEMENTED;
}

upm_result_t upm_mvs_is_colliding(void* dev, bool* collision_val){
    upm_mvs0608 device = (upm_mvs0608) dev;
    int value = 0, len = 1;
    upm_mvs0608_read(device, &value, len);
    if(!value)
        *collision_val = true;
    else
        *collision_val = false;
    return UPM_SUCCESS;
}
