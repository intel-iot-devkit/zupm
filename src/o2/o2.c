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

#include "o2.h"
#include "mraa/aio.h"

/**
 * Analog sensor struct
 */
typedef struct _o2_context {
    /* mraa aio pin context */
    mraa_aio_context aio;
    /* Analog voltage reference */
    float m_aRef;
    /* Raw count offset */
    float m_count_offset;
    /* Raw count scale */
    float m_count_scale;
} *o2_context;

o2_context o2_init(int16_t pin)
{
    o2_context dev = (o2_context)malloc(sizeof(struct _o2_context));

    if(dev == NULL) return NULL;

    /* Init aio pin */
    dev->aio = mraa_aio_init(pin);
    if(dev->aio == NULL) {
        free(dev);
        return NULL;
    }

    /* Set the ref, zero the offset */
    dev->m_count_offset = 0.0;
    dev->m_count_scale = 1.0;

    return dev;
}

void o2_close(o2_context dev)
{
    mraa_aio_close(dev->aio);
    free(dev);
}

upm_result_t o2_set_offset(const o2_context dev, float offset)
{
    dev->m_count_offset = offset;
    return UPM_SUCCESS;
}

upm_result_t o2_set_scale(const o2_context dev, float scale)
{
    dev->m_count_scale = scale;
    return UPM_SUCCESS;
}

upm_result_t o2_get_value(const o2_context dev, float *value)
{
    /* Read counts */
    int counts = mraa_aio_read(dev->aio);

    /* Apply raw scale */
    *value = counts * dev->m_count_scale;

    /* Apply raw offset */
    *value += dev->m_count_offset * dev->m_count_scale;

    return UPM_SUCCESS;
}
