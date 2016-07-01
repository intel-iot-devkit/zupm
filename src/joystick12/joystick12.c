/*
 * Author:
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

#include <string.h>
#include <stdlib.h>

#include "mraa/aio.h"
#include "types/upm_sensor.h"
#include "types/upm_joystick.h"
#include "joystick12.h"

const char upm_joystick12_name[] = "JOYSTICK12";
const char upm_joystick12_description[] = "Analog joystick sensor";
const upm_protocol_t upm_joystick12_protocol[] = {UPM_ANALOG};
const upm_sensor_t upm_joystick12_category[] = {UPM_JOYSTICK};

/**
 * Analog sensor struct
 */
typedef struct _upm_joystick12 {
    /* mraa ai_x pin context */
    mraa_aio_context ai_x;
    /* mraa ai_y pin context */
    mraa_aio_context ai_y;
    /* Raw count offset - x axis */
    float m_count_offset_x;
    /* Raw count offset - y axis */
    float m_count_offset_y;
    /* Raw count scale - x axis */
    float m_count_scale_x;
    /* Raw count scale - y axis */
    float m_count_scale_y;
} upm_joystick12;

/* This sensor implementes 2 function tables */
/* 1. Generic base function table */
static const upm_sensor_ft ft_gen =
{
    .upm_sensor_init_name = &upm_joystick12_init_str,
    .upm_sensor_close = &upm_joystick12_close,
    .upm_sensor_read = &upm_joystick12_read,
    .upm_sensor_write = &upm_joystick12_write,
    .upm_sensor_get_descriptor = &upm_joystick12_get_descriptor
};

/* 2. JOYSTICK function table */
static const upm_joystick_ft ft_joystick =
{
    .upm_joystick_set_offset_x = &upm_joystick12_set_offset_x,
    .upm_joystick_set_offset_y = &upm_joystick12_set_offset_y,
    .upm_joystick_set_scale_x = &upm_joystick12_set_scale_x,
    .upm_joystick_set_scale_y = &upm_joystick12_set_scale_y,
    .upm_joystick_get_value_x = &upm_joystick12_get_value_x,
    .upm_joystick_get_value_y = &upm_joystick12_get_value_y
};

const void* upm_joystick12_get_ft(upm_sensor_t sensor_type)
{
    switch(sensor_type)
    {
        case UPM_SENSOR:
            return &ft_gen;
        case UPM_JOYSTICK:
            return &ft_joystick;
        default:
            return NULL;
    }
}

void* upm_joystick12_init_str(const char* protocol, const char* params)
{
    fprintf(stderr, "String initialization - not implemented, using ain0 = x ain1 = y: %s\n", __FILENAME__);
    return upm_joystick12_init(0, 1);
}

void* upm_joystick12_init(int16_t apin_x, int16_t apin_y)
{
    upm_joystick12* dev = (upm_joystick12*) malloc(sizeof(upm_joystick12));

    if(dev == NULL) return NULL;

    /* Init analog pins */
    dev->ai_x = mraa_aio_init(apin_x);
    dev->ai_y = mraa_aio_init(apin_y);

    /* Set the ref, zero the offset */
    dev->m_count_offset_x = 0.0;
    dev->m_count_scale_x = 1.0;
    dev->m_count_offset_y = 0.0;
    dev->m_count_scale_y = 1.0;

    if((dev->ai_x == NULL) || (dev->ai_y == NULL))
    {
        free(dev);
        return NULL;
    }

    return dev;
}

void upm_joystick12_close(void* dev)
{
    mraa_aio_close(((upm_joystick12*)dev)->ai_x);
    mraa_aio_close(((upm_joystick12*)dev)->ai_y);
    free(dev);
}

const upm_sensor_descriptor_t upm_joystick12_get_descriptor()
{
    /* Fill in the descriptor */
    upm_sensor_descriptor_t usd;
    usd.name = upm_joystick12_name;
    usd.description = upm_joystick12_description;
    usd.protocol_size = 1;
    usd.protocol = upm_joystick12_protocol;
    usd.category_size = 1;
    usd.category = upm_joystick12_category;

    return usd;
}

upm_result_t upm_joystick12_read(const void* dev, void* value, int len)
{
    /* Read the adc twice, first adc read can have weird data */
    mraa_aio_read(((upm_joystick12*)dev)->ai_x);
    mraa_aio_read(((upm_joystick12*)dev)->ai_y);
    ((int*)value)[0] = mraa_aio_read(((upm_joystick12*)dev)->ai_x);
    ((int*)value)[1] = mraa_aio_read(((upm_joystick12*)dev)->ai_y);
    if (value < 0)
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t upm_joystick12_write(const void* dev, void* value, int len)
{
    return UPM_ERROR_NOT_SUPPORTED;
}

upm_result_t upm_joystick12_set_offset_x(const void* dev, float offset)
{
    ((upm_joystick12*)dev)->m_count_offset_x = offset;
    return UPM_SUCCESS;
}

upm_result_t upm_joystick12_set_offset_y(const void* dev, float offset)
{
    ((upm_joystick12*)dev)->m_count_offset_y = offset;
    return UPM_SUCCESS;
}

upm_result_t upm_joystick12_set_scale_x(const void* dev, float scale)
{
    ((upm_joystick12*)dev)->m_count_scale_x = scale;
    return UPM_SUCCESS;
}

upm_result_t upm_joystick12_set_scale_y(const void* dev, float scale)
{
    ((upm_joystick12*)dev)->m_count_scale_y = scale;
    return UPM_SUCCESS;
}

upm_result_t upm_joystick12_get_value_x(const void* dev, float *value)
{
    int counts[2] = {0,0};

    /* Read counts from the generic read method */
    upm_joystick12_read(dev, &counts, 2);

    /* Get max adc value range 1023, 2047, 4095, etc... */
    float max_adc = (1 << mraa_aio_get_bit(((upm_joystick12*)dev)->ai_x)) - 1;

    /* Apply raw offset */
    *value = counts[0] + ((upm_joystick12*)dev)->m_count_offset_x;

    // Scale to -1.0 +1.0
    *value =  -2.0 * (((max_adc/2.0 - *value))/max_adc * ((upm_joystick12*)dev)->m_count_scale_x);

    return UPM_SUCCESS;
}

upm_result_t upm_joystick12_get_value_y(const void* dev, float *value)
{
    int counts[2] = {0,0};

    /* Read counts from the generic read method */
    upm_joystick12_read(dev, &counts, 2);

    /* Get max adc value range 1023, 2047, 4095, etc... */
    float max_adc = (1 << mraa_aio_get_bit(((upm_joystick12*)dev)->ai_y)) - 1;

    /* Apply raw offset */
    *value = counts[1] + ((upm_joystick12*)dev)->m_count_offset_y;

    // Scale to -1.0 +1.0
    *value =  -2.0 * (((max_adc/2.0 - *value))/max_adc * ((upm_joystick12*)dev)->m_count_scale_y);

    return UPM_SUCCESS;
}

upm_result_t upm_joystick12_zero(const void* dev)
{
    int counts[2] = {0, 0};

    /* Read counts from the generic read method */
    upm_joystick12_read(dev, &counts, 2);

    /* Get max adc value range 1023, 2047, 4095, etc... */
    float max_adc = (1 << mraa_aio_get_bit(((upm_joystick12*)dev)->ai_y)) - 1;

    ((upm_joystick12*)dev)->m_count_offset_x = max_adc/2.0 - counts[0];
    ((upm_joystick12*)dev)->m_count_offset_y = max_adc/2.0 - counts[1];

    return UPM_SUCCESS;
}

upm_result_t upm_joystick12_calibrate_x(const void* dev)
{
    int counts[2] = {0, 0};

    /* Read counts from the generic read method */
    upm_joystick12_read(dev, &counts, 2);

    /* Get max adc value range 1023, 2047, 4095, etc... */
    float max_adc = (1 << mraa_aio_get_bit(((upm_joystick12*)dev)->ai_y)) - 1;

    float fcounts = counts[0];

    /* no need to scale if counts is a max or min value */
    if ((fcounts == 0) || (fcounts == max_adc)) return UPM_SUCCESS;

    //((upm_joystick12*)dev)->m_count_scale_x = .5 * max_adc/fcounts;
    ((upm_joystick12*)dev)->m_count_scale_x =
        1.0 / (1.0 - 2 * fcounts/max_adc);

}

upm_result_t upm_joystick12_calibrate_y(const void* dev)
{
    int counts[2] = {0, 0};

    /* Read counts from the generic read method */
    upm_joystick12_read(dev, &counts, 2);

    /* Get max adc value range 1023, 2047, 4095, etc... */
    float max_adc = (1 << mraa_aio_get_bit(((upm_joystick12*)dev)->ai_y)) - 1;

    float fcounts = counts[1];

    /* no need to scale if counts is a max or min value */
    if ((fcounts == 0) || (fcounts == max_adc)) return UPM_SUCCESS;

    ((upm_joystick12*)dev)->m_count_scale_y = .5 * max_adc/fcounts;
}
