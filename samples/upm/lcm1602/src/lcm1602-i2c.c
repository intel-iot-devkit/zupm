/*
 * Author: Jon Trulson <jtrulson@ics.com>
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

#include "lcm1602.h"
#include <upm_utilities.h>

int main(int argc, char **argv)
{
//! [Interesting]
    lcm1602_context lcd = lcm1602_i2c_init(0, 0x20, true, 16, 2);

    if (!lcd)
    {
        printf("lcm1602_i2c_init() failed\n");
        return 1;
    }

    lcm1602_set_cursor(lcd, 0, 0);
    lcm1602_write(lcd, "Hello World 1", 13);

    upm_delay(3);

    lcm1602_set_cursor(lcd, 1, 0);
    lcm1602_write(lcd, "Hello World 2", 13);

    upm_delay(3);

    lcm1602_close(lcd);
//! [Interesting]

    return 0;
}
