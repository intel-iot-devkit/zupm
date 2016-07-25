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

#include "mq303a.h"
#include "mraa/aio.h"

typedef struct _mq303a_context {
    mraa_gpio_context       gpio;
    mraa_aio_context        aio;
    uint8_t                 gpio_pin;
    uint8_t                 aio_pin;
} *mq303a_context;

#if defined(CONFIG_BOARD_ARDUINO_101) || defined(CONFIG_BOARD_ARDUINO_101_SSS) || defined(CONFIG_BOARD_QUARK_D2000_CRB)
DEFINE_MEM_MAP(UPM_MQ303A_MEM_MAP, 1, sizeof(struct _upm_mq303a));
const kmemory_map_t UPM_MQ303A_MEM_MAP;
#elif defined(linux)
#define UPM_MQ303A_MEM_MAP 0
#endif

mq303a_context upm_mq303a_init(int pin, int heater_pin){
    mq303a_context dev =
      (mq303a_context)upm_malloc(UPM_MQ303A_MEM_MAP,
                                 sizeof(struct _mq303a_context));

    if (!dev)
      return NULL;

    dev->aio_pin = pin;
    dev->gpio_pin = heater_pin;
    dev->aio = mraa_aio_init(dev->aio_pin);
    dev->gpio = mraa_gpio_init(dev->gpio_pin);

    if(mraa_gpio_dir(dev->gpio, MRAA_GPIO_OUT) != MRAA_SUCCESS)
      {
        upm_free(UPM_MQ303A_MEM_MAP, dev);
        return NULL;
      }

    return dev;
}

void mq303a_close(mq303a_context dev){
    upm_free(UPM_MQ303A_MEM_MAP, dev);
}

upm_result_t mq303a_heater_enable(mq303a_context dev, bool enable){
    if(enable)
        mraa_gpio_write(dev->gpio, 0);
    else
        mraa_gpio_write(dev->gpio, 1);

    return UPM_SUCCESS;
}

upm_result_t mq303a_get_value(mq303a_context dev, int* val){
    *val = mraa_aio_read(dev->aio);

    if (*val < 0)
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}
