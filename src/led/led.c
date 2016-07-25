/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Contributions: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Contributions: Sarah Knepper <sarah.knepper@intel.com>
 *                Abhishek Malik <abhishek.malik@intel.com>
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
#include "led.h"
#include "upm_types.h"
#include "mraa/gpio.h"

typedef struct _led_context {
    mraa_gpio_context        gpio;
    uint8_t                  led_pin;
} *led_context;

#if defined(CONFIG_BOARD_ARDUINO_101) || defined(CONFIG_BOARD_ARDUINO_101_SSS) || defined(CONFIG_BOARD_QUARK_D2000_CRB)
DEFINE_MEM_MAP(UPM_LED_MEM_MAP, 1, sizeof(struct _led_context));
const kmemory_map_t UPM_LED_MEM_MAP;
#elif defined(linux)
#define UPM_LED_MEM_MAP 0
#endif

led_context led_init(uint8_t pin){
    led_context dev =
      (led_context)upm_malloc(UPM_LED_MEM_MAP, sizeof(struct _led_context));

    if (!dev)
      return NULL;

    dev->led_pin = pin;
    dev->gpio = mraa_gpio_init(dev->led_pin);

    if (mraa_gpio_dir(dev->gpio, MRAA_GPIO_OUT) != MRAA_SUCCESS)
      return NULL;

    return dev;
}

void led_close(led_context dev){
    upm_free(UPM_LED_MEM_MAP, dev);
}

upm_result_t led_on(led_context dev){
    if (mraa_gpio_write(dev->gpio, 1) != MRAA_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t led_off(led_context dev){
    if (mraa_gpio_write(dev->gpio, 0) != MRAA_SUCCESS)
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}
