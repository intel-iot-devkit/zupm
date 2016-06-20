/*
 * Author: Jon Trulson <jtrulson@ics.com>
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

#include <iostream>

#include "dfrph.hpp"

using namespace upm;

DFRPH::DFRPH(int pin, float aref) : _dev(upm_dfrph_init(pin, aref)) {}

DFRPH::~DFRPH()
{
    upm_dfrph_close(_dev);
}

float DFRPH::volts()
{
    float volts = 0.0;
    _dev->m_aRef * upm_dfrph_get_value(_dev, &volts, NORMALIZED);
    return volts;
}

void DFRPH::setOffset(float offset)
{
    _dev->m_offset = offset;
}

float DFRPH::pH(unsigned int samples)
{
    float ph_avg = 0.0;

    // Read at least 1 sample
    if (samples == 0) samples = 1;

    float ph = 0.0;
    while (samples-- > 0)
    {
        _dev->m_aRef * upm_dfrph_get_value(_dev, &ph, PH);
        ph_avg += ph;
    }

    return ph_avg/samples;
}
