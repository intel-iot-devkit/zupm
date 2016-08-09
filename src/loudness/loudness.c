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
#include "loudness.h"

#if defined(CONFIG_BOARD_ARDUINO_101) || defined(CONFIG_BOARD_ARDUINO_101_SSS) || defined(CONFIG_BOARD_QUARK_D2000_CRB)
DEFINE_MEM_MAP(UPM_LOUDNESS_MAP, 1, sizeof(struct _upm_loudness));
const kmemory_map_t UPM_LOUDNESS_MEM_MAP;
#elif defined(linux)
#define UPM_LOUDNESS_MEM_MAP 0
#endif

loudness_context upm_loudness_init(int pin) {
    loudness_context dev =
      (loudness_context)upm_malloc(UPM_LOUDNESS_MEM_MAP,
                                   sizeof(struct _loudness_context));

    if (!dev)
      return NULL;

    dev->aio = mraa_aio_init(pin);

    if (!dev->aio)
      {
        free(dev);
        return NULL;
      }

    return dev;
}

void loudness_close(loudness_context dev) {
    mraa_aio_close(dev->aio);
    upm_free(UPM_LOUDNESS_MEM_MAP, dev);
}

upm_result_t loudness_get_value(loudness_context dev, int* val) {
    *val = mraa_aio_read(dev->aio);

    return UPM_SUCCESS;
}
