/*
 * Author:Sisinty Sasmita Patra <sisinty.s.patra@intel.com>
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

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mic.h"
#include "upm_utilities.h"
#include "mraa.h"

int main()
{
    if (mraa_init() != MRAA_SUCCESS)
    {
        fprintf(stderr,"Failed to initialize mraa\n");
        return -1;
    }

//! [Interesting]
    // Instantiate a Grove Mic Sensor, using analog pins A0
    mic_context dev = mic_init(0);

    float val;
    while(1) {
        mic_get_value(dev, &val);
#if defined(CONFIG_BOARD_QUARK_D2000_CRB)
        printf("mic value: %d\n", (int)val);
#elif defined(CONFIG_BOARD_ARDUINO_101_SSS)
        printf("mic value: %f\n", val);
#endif
        upm_delay(1);
    }

    mic_close(dev);
//! [Interesting]
    return 0;
}
