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

#if defined(CONFIG_BOARD_ARDUINO_101) || defined(CONFIG_BOARD_ARDUINO_101_SSS) || defined(CONFIG_BOARD_QUARK_D2000_CRB)
DEFINE_MEM_MAP(UPM_MVS0608_MEM_MAP, 1, sizeof(struct _mvs0608_context));
const kmemory_map_t UPM_MVS0608_MEM_MAP;
#elif defined(linux)
#define UPM_MVS0608_MEM_MAP 0
#endif

mvs0608_context upm_mvs0608_init(int pin){
    mvs0608_context dev =
      (mvs0608_context)upm_malloc(UPM_MVS0608_MEM_MAP,
                                  sizeof(struct _mvs0608_context));

    if (!dev)
      return NULL;

    dev->gpio_pin = pin;
    dev->gpio = mraa_gpio_init(dev->gpio_pin);

    if(mraa_gpio_dir(dev->gpio, MRAA_GPIO_IN) != MRAA_SUCCESS)
      {
        upm_free(UPM_MVS0608_MEM_MAP, dev);
        return NULL;
      }

    return dev;
}

void mvs0608_close(mvs0608_context dev){
    upm_free(UPM_MVS0608_MEM_MAP, dev);
}

upm_result_t mvs0608_is_colliding(mvs0608_context dev, bool* collision_val){
    int value = mraa_gpio_read(dev->gpio);

    if(!value)
        *collision_val = true;
    else
        *collision_val = false;

    return UPM_SUCCESS;
}
