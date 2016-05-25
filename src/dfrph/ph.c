/*
 * Author: Sisinty Sasmita Patra <sisinty.s.patra@intel.com>
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

#include "ph.h"

void get_upm_descriptor(upm_sensor_descriptor* desc)
{
    strcpy(desc->name, "dfrph");
    strcpy(desc->description, "Analog pH Meter Pro");
    desc->category[0] = UPM_PH;
}

upm_ft_ph upm_get_ph_ft()
{
    /* Fill in the function table for this sensor */
    upm_ft_ph ft;
    ft.get_upm_descriptor = &get_upm_descriptor;
    ft.upm_ph_init = &upm_dfrph_init;
    ft.upm_ph_set_offset = &upm_dfrph_set_offset;
    ft.upm_ph_get_value = &upm_dfrph_get_value;
    return ft;
}

upm_ph upm_dfrph_init(int16_t pin, float aref)
{
    upm_ph dev = (upm_ph) malloc(sizeof(struct _upm_ph));

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

void upm_dfrph_close(upm_ph dev)
{
    mraa_aio_close(dev->aio);
    free(dev);
}

upm_result_t upm_dfrph_set_offset(upm_ph dev, float ph_offset)
{
    dev->m_offset = ph_offset;
    return UPM_SUCCESS;
}

upm_result_t upm_dfrph_get_value(upm_ph dev, float *value, upm_ph_value_t unit)
{
//    printf("XXX value context: aRes: %d aRef: %f offset: %f\n",  dev->m_aRes, dev->m_aRef, dev->m_offset);
//    return;
    /* Read the adc, first adc read can have weird data */
    mraa_aio_read(dev->aio);
    int counts = mraa_aio_read(dev->aio);

    /* Provide the value in whichever unit was requested */
    switch (unit)
    {
        case UPM_PH_RAW:
            *value = counts;
            break;
        case UPM_PH_NORMALIZED:
            *value = counts / (float)dev->m_aRes;
            break;
        case UPM_PH_PH:
            *value = 3.5 * dev->m_aRef * counts / dev->m_aRes + dev->m_offset;
            break;
        default:
            //upm_perror("Invalid pH units specified: ", unit);
            break;
    }

    return UPM_SUCCESS;
}
