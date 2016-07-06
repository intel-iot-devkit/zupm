/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 *            Abhishek Malik <abhishek.malik@intel.com>
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

#include "es9257.h"

struct _upm_es9257 {
    mraa_pwm_context    pwm;
    uint16_t            servo_pin;
    uint32_t            max_pulse_width;
    uint32_t            min_pulse_width;
};

const char upm_es9257_name[] = "ES9257";
const char upm_es9257_description[] = "Grove Servo - Heavy Duty";
const upm_protocol_t upm_es9257_protocol[] = {UPM_PWM};
const upm_sensor_t upm_es9257_category[] = {UPM_SERVO};

upm_sensor_descriptor_t upm_es9257_get_descriptor (void* dev){
    upm_sensor_descriptor_t usd;
    usd.name = upm_es9257_name;
    usd.description = upm_es9257_description;
    usd.protocol_size = 1;
    usd.protocol = upm_es9257_protocol;
    usd.category_size = 1;
    usd.category = upm_es9257_category;
    return usd;
}

void* upm_es9257_get_ft(upm_sensor_t sensor_type){
    if(sensor_type == UPM_SERVO){
        upm_servo_ft *sft = malloc(sizeof(*sft));
        if(sft == NULL){
            printf("Unable to assign memory to the Servo motor structure");
            return NULL;
        }
        sft->upm_servo_set_angle = upm_es9257_set_angle;
        return sft;
    }
    else if(sensor_type == UPM_SENSOR){
        upm_sensor_ft *ft = malloc(sizeof(*ft));
        if(ft == NULL){
            printf("Unable to assign memory to the Servo motor structure");
            return NULL;
        }
        ft->upm_sensor_init_name = upm_es9257_init_name;
        ft->upm_sensor_close = upm_es9257_close;
        ft->upm_sensor_write = upm_es9257_write;
        ft->upm_sensor_read = upm_es9257_read;
    }
    return NULL;
}

// VA used only temporarily until MRAA implements character based init
void* upm_es9257_init_name(int num,...){
    upm_es9257 dev = (upm_es9257) malloc(sizeof(struct _upm_es9257));
    //
    va_list pin_list;
    printf("Coming into the es9257 function\n");

    //
    va_start(pin_list, num);
    if(dev == NULL){
        printf("Unable to assign memory to the Servo motor structure");
        return NULL;
    }

    // first argument is the pin number
    dev->servo_pin = va_arg(pin_list, int);
    // second is the min pulse width
    dev->min_pulse_width = va_arg(pin_list, int);
    // third is the max pulse width
    dev->max_pulse_width = va_arg(pin_list, int);

    va_end(pin_list);
    dev->pwm = mraa_pwm_init(dev->servo_pin);
    if(dev->pwm == NULL){
        printf("Unable to initialize the PWM pin");
    }

    upm_es9257_set_angle(dev, 0);
    return dev;
}

void* upm_es9257_init(int32_t pin, int32_t min_pulse_width, int32_t max_pulse_width){
    upm_es9257 dev = (upm_es9257) malloc(sizeof(struct _upm_es9257));
    if(dev == NULL){
        printf("Unable to assign memory to the Servo motor structure");
        return NULL;
    }

    // first argument is the pin number
    dev->servo_pin = pin;
    // second is the min pulse width
    dev->min_pulse_width = min_pulse_width;
    // third is the max pulse width
    dev->max_pulse_width = max_pulse_width;

    dev->pwm = mraa_pwm_init(dev->servo_pin);
    if(dev->pwm == NULL){
        printf("Unable to initialize the PWM pin");
    }

    upm_es9257_set_angle(dev, 0);
    return dev;
}

void upm_es9257_close(void* dev){
    upm_es9257 device = (upm_es9257) dev;
    mraa_pwm_enable(device->pwm, 0);
    free(device);
}

upm_result_t upm_es9257_write (void* dev, void* data, int len){
    upm_es9257 device = (upm_es9257) dev;
    uint32_t int_data = *((uint32_t *) data);
    if(UPM_ES9257_MAX_ANGLE < int_data || int_data < 0){
        printf("The angle specified is either above the max angle or below 0");
        return UPM_ERROR_UNSPECIFIED;
    }
    printf("setting angle to: %d\n", int_data);

    mraa_pwm_enable(device->pwm, 1);
    mraa_pwm_period_us(device->pwm, UPM_ES9257_PERIOD);
    int32_t val = 0;

    upm_es9257_calc_pulse_travelling(dev, &val, int_data);
    mraa_pwm_pulsewidth_us(device->pwm, val);

    upm_delay(1);
    mraa_pwm_enable(device->pwm, 0);

    return UPM_SUCCESS;
    //return upm_es08a_set_angle(device, angle);
}

upm_result_t upm_es9257_set_angle(void* dev, int32_t angle){
    upm_es9257 device = (upm_es9257) dev;
    upm_es9257_write(device, &angle, 0);
    return UPM_SUCCESS;
}

upm_result_t upm_es9257_calc_pulse_travelling(void* dev, int32_t* ret_val, int32_t value){
    upm_es9257 device = (upm_es9257) dev;
    if (value > device->max_pulse_width) {
        *ret_val = device->max_pulse_width;
    }

    // if less than the boundaries
    if (value  < 0) {
        *ret_val = device->min_pulse_width;
    }

    *ret_val = (int) ((float)device->min_pulse_width + ((float)value / UPM_ES9257_MAX_ANGLE) * ((float)device->max_pulse_width - (float)device->min_pulse_width));
    return UPM_SUCCESS;
}

void upm_es9257_set_min_pulse_width (void* dev, int width){
    upm_es9257 device = (upm_es9257) dev;
    device->min_pulse_width = width;
}

void upm_es9257_set_max_pulse_width (void* dev, int width){
    upm_es9257 device = (upm_es9257) dev;
    device->max_pulse_width = width;
}

int upm_es9257_get_min_pulse_width (void* dev){
    upm_es9257 device = (upm_es9257) dev;
    return device->min_pulse_width;
}

int upm_es9257_get_max_pulse_width (void* dev){
    upm_es9257 device = (upm_es9257) dev;
    return device->max_pulse_width;
}

upm_result_t upm_es9257_read(void* dev, void* value, int* len){
    return UPM_ERROR_NOT_IMPLEMENTED;
}
