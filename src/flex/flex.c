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

#include "flex.h"

flex_context flex_init(int16_t pin)
{
    flex_context dev = (flex_context) malloc(sizeof(struct _flex_context));

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

void flex_close(flex_context dev)
{
    mraa_aio_close(dev->aio);
    free(dev);
}

upm_result_t flex_set_offset(const flex_context dev, float offset)
{
    dev->m_count_offset = offset;
    return UPM_SUCCESS;
}

upm_result_t flex_set_scale(const flex_context dev, float scale)
{
    dev->m_count_scale = scale;
    return UPM_SUCCESS;
}

upm_result_t flex_get_value(const flex_context dev, float *value)
{
    int counts = mraa_aio_read(dev->aio);

    if (counts < 0)
        return UPM_ERROR_OPERATION_FAILED;

    /* Apply raw scale */
    *value = counts * dev->m_count_scale;

    /* Apply raw offset */
    *value += dev->m_count_offset * dev->m_count_scale;

    return UPM_SUCCESS;
}
