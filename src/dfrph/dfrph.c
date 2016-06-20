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

#include "dfrph.h"

const char upm_dfrph_name[] = "DFRPH";
const char upm_dfrph_description[] = "Analog pH Meter Pro";
const upm_protocol_t upm_dfrph_protocol[] = {UPM_ANALOG};
const upm_sensor_t upm_dfrph_category[] = {UPM_PH};

upm_dfrph* upm_dfrph_init_str(const char* protocol, const char* params)
{
    return NULL;
}

upm_dfrph* upm_dfrph_init(int16_t pin, float aref)
{
    upm_dfrph* dev = (upm_dfrph*) malloc(sizeof(upm_dfrph));

    if(dev == NULL) return NULL;

    /* Init aio pin */
    dev->aio = mraa_aio_init(pin);

    /* Get adc bit range */
    dev->m_aRes = (1 << mraa_aio_get_bit(dev->aio));

    /* Set the ref, zero the offset */
    dev->m_aRef = aref;
    dev->m_offset = 0.0;

    if(dev->aio == NULL) {
        free(dev);
        return NULL;
    }

    return dev;
}

void upm_dfrph_close(upm_dfrph* dev)
{
    mraa_aio_close(dev->aio);
    free(dev);
}

upm_sensor_ft upm_get_ft()
{
    /* Fill in the function table */
    upm_sensor_ft ft;
    ft.upm_sensor_init_name = &upm_dfrph_init_str;
    ft.upm_sensor_close = &upm_dfrph_close;
    ft.upm_sensor_read = &upm_dfrph_read;
    ft.upm_sensor_write = &upm_dfrph_write;
    ft.upm_sensor_get_descriptor = &upm_dfrph_get_descriptor;
    return ft;
}

const upm_sensor_descriptor_t upm_dfrph_get_descriptor (void* dev)
{
    /* Fill in the descriptor */
    upm_sensor_descriptor_t usd;
    usd.name = upm_dfrph_name;
    usd.description = upm_dfrph_description;
    usd.protocol_size = 1;
    usd.protocol = upm_dfrph_protocol;
    usd.category_size = 1;
    usd.category = upm_dfrph_category;

    return usd;
}

upm_result_t upm_dfrph_read (void* dev, void* value, int len)
{
    /* Read the adc twice, first adc read can have weird data */
    mraa_aio_read(((upm_dfrph*)dev)->aio);
    *(int*)value = mraa_aio_read(((upm_dfrph*)dev)->aio);
    if (value < 0)
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t upm_dfrph_write (void* dev, void* value, int len)
{
    return UPM_ERROR_NOT_SUPPORTED;
}

upm_result_t upm_dfrph_get_value(upm_dfrph* dev, float *value, upm_ph_u unit)
{
    int counts = 0;

    /* Read counts from the generic read method */
    upm_dfrph_read(dev, counts, 1);

    /* Provide the value in whichever unit was requested */
    switch (unit)
    {
        case NORMALIZED:
            *value = counts / (float)dev->m_aRes;
            break;
        case PH:
            *value = 3.5 * dev->m_aRef * counts / dev->m_aRes + dev->m_offset;
            break;
        default:
            //upm_perror("Invalid pH units specified: ", unit);
            break;
    }

    return UPM_SUCCESS;
}

upm_result_t upm_dfrph_set_offset(upm_dfrph* dev, float ph_offset)
{
    dev->m_offset = ph_offset;
    return UPM_SUCCESS;
}
