/*
 * Author: Noel Eck <noel.eck@intel.com>
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

#include <unistd.h>
#include <signal.h>
#include <stdio.h>

#include "dfrph.h"
#include "upm_utilities.h"
#include "mraa.h"

bool shouldRun = true;

void sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}

int main()
{
upm_delay_ms(500);
    if (mraa_init() != MRAA_SUCCESS)
    {
        fprintf(stderr,"Failed to initialize mraa\n");
        return -1;
    }

    signal(SIGINT, sig_handler);

    //! [Interesting]

    // Instantiate a dfrph sensor on analog pin A0
    dfrph_context sensor = dfrph_init(0);

    if (!sensor)
    {
        printf("dfrph_init() failed.\n");
        return(1);
    }

    // Set the ADC reference to 3.3 for the Arduino 101
    dfrph_set_aref(sensor, 3.3);

    // Every half a second, sample the sensor output
    while (shouldRun)
    {
        float volts = 0.0, pH = 0.0;

        dfrph_get_raw_volts(sensor, &volts);
        dfrph_get_ph(sensor, &pH);
#if defined(CONFIG_BOARD_ARDUINO_101_SSS)
        printf("Detected volts: %0.03f\n", volts);
        printf("pH value: %0.03f\n", pH);
#elif defined(CONFIG_BOARD_QUARK_D2000_CRB)
        printf("Detected millivolts: %d\n", (int)(volts*1000));
        printf("pH value: %d\n", (int)pH);
#endif
        upm_delay_ms(500);
    }

    //! [Interesting]

    printf("Exiting\n");

    dfrph_close(sensor);

    return 0;
}
