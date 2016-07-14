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
#include "types/upm_raw.h"
#include "mq7.h"

const char upm_mq7_name[] = "MQ7";
const char upm_mq7_description[] = "Analog gas sensor (mq7) sensor";
const upm_protocol_t upm_mq7_protocol[] = {UPM_ANALOG};
const upm_sensor_t upm_mq7_category[] = {UPM_RAW};

/**
 * Analog sensor struct
 */
typedef struct _upm_mq7 {
    /* mraa aio pin context */
    mraa_aio_context aio;
    /* Analog voltage reference */
    float m_aRef;
    /* Raw count offset */
    float m_count_offset;
    /* Raw count scale */
    float m_count_scale;
} upm_mq7;

/* This sensor implementes 2 function tables */
/* 1. Generic base function table */
static const upm_sensor_ft ft_gen =
{
    .upm_sensor_init_name = &upm_mq7_init_str,
    .upm_sensor_close = &upm_mq7_close,
    .upm_sensor_read = &upm_mq7_read,
    .upm_sensor_write = &upm_mq7_write,
    .upm_sensor_get_descriptor = &upm_mq7_get_descriptor
};

/* 2. RAW function table */
static const upm_raw_ft ft_raw =
{
    .upm_raw_set_offset = &upm_mq7_set_offset,
    .upm_raw_set_scale = &upm_mq7_set_scale,
    .upm_raw_get_value = &upm_mq7_get_value
};

const void* (*upm_get_ft) (upm_sensor_t sensor_type) = &upm_mq7_get_ft;

const void* upm_mq7_get_ft(upm_sensor_t sensor_type)
{
    switch(sensor_type)
    {
        case UPM_SENSOR:
            return &ft_gen;
        case UPM_RAW:
            return &ft_raw;
        default:
            return NULL;
    }
}

void* upm_mq7_init_str(const char* protocol, const char* params)
{
    fprintf(stderr, "String initialization - not implemented, using ain0: %s\n", __FILENAME__);
    return upm_mq7_init(0);
}

void* upm_mq7_init(int16_t pin)
{
    upm_mq7* dev = (upm_mq7*) malloc(sizeof(upm_mq7));

    if(dev == NULL) return NULL;

    /* Init aio pin */
    dev->aio = mraa_aio_init(pin);

    /* Set the ref, zero the offset */
    dev->m_count_offset = 0.0;
    dev->m_count_scale = 1.0;

    if(dev->aio == NULL) {
        free(dev);
        return NULL;
    }

    return dev;
}

void upm_mq7_close(void* dev)
{
    mraa_aio_close(((upm_mq7*)dev)->aio);
    free(dev);
}

const upm_sensor_descriptor_t upm_mq7_get_descriptor()
{
    /* Fill in the descriptor */
    upm_sensor_descriptor_t usd;
    usd.name = upm_mq7_name;
    usd.description = upm_mq7_description;
    usd.protocol_size = 1;
    usd.protocol = upm_mq7_protocol;
    usd.category_size = 1;
    usd.category = upm_mq7_category;

    return usd;
}

upm_result_t upm_mq7_read(const void* dev, void* value, int len)
{
    /* Read the adc twice, first adc read can have weird data */
    mraa_aio_read(((upm_mq7*)dev)->aio);
    *(int*)value = mraa_aio_read(((upm_mq7*)dev)->aio);
    if (value < 0)
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t upm_mq7_write(const void* dev, void* value, int len)
{
    return UPM_ERROR_NOT_SUPPORTED;
}

upm_result_t upm_mq7_set_offset(const void* dev, float offset)
{
    ((upm_mq7*)dev)->m_count_offset = offset;
    return UPM_SUCCESS;
}

upm_result_t upm_mq7_set_scale(const void* dev, float scale)
{
    ((upm_mq7*)dev)->m_count_scale = scale;
    return UPM_SUCCESS;
}

upm_result_t upm_mq7_get_value(const void* dev, float *value)
{
    int counts = 0;

    /* Read counts from the generic read method */
    upm_mq7_read(dev, &counts, 1);

    /* Get max adc value range 1023, 2047, 4095, etc... */
    float max_adc = (1 << mraa_aio_get_bit(((upm_mq7*)dev)->aio)) - 1;

    /* Apply raw scale */
    *value = counts * ((upm_mq7*)dev)->m_count_scale;

    /* Apply raw offset */
    *value += ((upm_mq7*)dev)->m_count_offset *((upm_mq7*)dev)->m_count_scale;

    return UPM_SUCCESS;
}
