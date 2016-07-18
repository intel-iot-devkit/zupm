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

#include "ad8232.h"

struct _upm_ad8232 {
    mraa_aio_context        aio;
    mraa_gpio_context       gpio_lo_plus;
    mraa_gpio_context       gpio_lo_minus;
    uint8_t                 aio_pin;
    uint8_t                 gpio_lo_plus_pin;
    uint8_t                 gpio_lo_minus_pin;
    int                     a_res;
};

const char upm_ad8232_name[] = "AD8232";
const char upm_ad8232_description[] = "AD8232 Heart Rate Monitor";
const upm_protocol_t upm_ad8232_protocol[] = {UPM_ANALOG, UPM_GPIO};
const upm_sensor_t upm_ad8232_category[] = {UPM_HEART_RATE};


upm_sensor_descriptor_t upm_ad8232_get_descriptor (void* dev){
    upm_sensor_descriptor_t usd;
    usd.name = upm_ad8232_name;
    usd.description = upm_ad8232_description;
    usd.protocol_size = 2;
    usd.protocol = upm_ad8232_protocol;
    usd.category_size = 1;
    usd.category = upm_ad8232_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_ad8232_init_name,
    .upm_sensor_close = &upm_ad8232_close,
    //.upm_sensor_get_descriptor = &upm_ad8232_get_descriptor
};

static const upm_heart_rate_ft hft =
{
    .upm_heart_rate_get_value = &upm_ad8232_get_value
};

#if defined(FRAMEWORK_BUILD)
typedef const void* (*upm_get_ft) (upm_sensor_t sensor_type);

upm_get_ft upm_assign_ft(){
    return upm_ad8232_get_ft;
}
#endif

const void* upm_ad8232_get_ft(upm_sensor_t sensor_type){
    if(sensor_type == UPM_HEART_RATE){
        return &hft;
    }
    else if(sensor_type == UPM_SENSOR){
        return &ft;
    }
}

void* upm_ad8232_init_name(){
    return NULL;
}

void* upm_ad8232_init(int lo_plus, int lo_minus, int output, float a_ref){
    upm_ad8232 dev = (upm_ad8232) malloc(sizeof(struct _upm_ad8232));
    if(dev == NULL){
        printf("Unable to assign memory to the Heart Rate Monitor structure");
        return NULL;
    }

    dev->gpio_lo_plus_pin = lo_plus;
    dev->gpio_lo_minus_pin = lo_minus;
    dev->aio_pin = output;

    dev->aio = mraa_aio_init(dev->aio_pin);
    dev->gpio_lo_plus = mraa_gpio_init(dev->gpio_lo_plus_pin);
    dev->gpio_lo_minus = mraa_gpio_init(dev->gpio_lo_minus_pin);

    if (dev->aio == NULL || dev->gpio_lo_minus == NULL ||
        dev->gpio_lo_plus == NULL){
        printf("The pins did not initialize correctly");
        return NULL;
    }

    if (mraa_gpio_dir(dev->gpio_lo_minus, MRAA_GPIO_IN) != MRAA_SUCCESS ||
        mraa_gpio_dir(dev->gpio_lo_plus, MRAA_GPIO_IN) != MRAA_SUCCESS){
        printf("Couldn't set the direction for the GPIO pins");
        return NULL;
    }

    dev->a_res = (1 << mraa_aio_get_bit(dev->aio));
    return dev;
}

void upm_ad8232_close(void* dev){
    upm_ad8232 device = (upm_ad8232) dev;
    if(device->gpio_lo_minus != NULL)
        mraa_gpio_close(device->gpio_lo_minus);
    if(device->gpio_lo_plus != NULL)
        mraa_gpio_close(device->gpio_lo_plus);
    if(device->aio != NULL)
        mraa_aio_close(device->aio);
    if(device != NULL)
        free(device);
}

upm_result_t upm_ad8232_get_value(void* dev, int* value,
                                  upm_heart_rate_u rate_unit){

    upm_ad8232 device = (upm_ad8232) dev;
    int len;
    int reading = 0;

    if (mraa_gpio_read(device->gpio_lo_minus) ||
        mraa_gpio_read(device->gpio_lo_plus)) {
        reading = 0;
    }
    else{
        reading = mraa_aio_read(device->aio);
    }

    *value = reading;

    return UPM_SUCCESS;
}

