/*
 * Author: Zion Orent <zorent@ics.com>
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
#include <string>
#include <stdexcept>

#include "o2.hpp"

using namespace upm;
using namespace std;

O2::O2(int pin)
{
    if ( !(m_dev = o2_init(pin)) )
    {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                ": o2_init() failed, invalid pin?");
        return;
    }
}

O2::~O2()
{
    o2_close(m_dev);
}

void O2::setAref(float aref)
{
    o2_set_aref(m_dev, aref);
}

float O2::getAref()
{
    return o2_get_aref(m_dev);
}

void O2::setRawOffset(float raw_offset)
{
    o2_set_offset(m_dev, raw_offset);
}

float O2::getRawOffset()
{
    o2_get_offset(m_dev);
}

void O2::setRawScale(float raw_scale)
{
    o2_set_scale(m_dev, raw_scale);
}

float O2::getRawScale()
{
    o2_get_scale(m_dev);
}

int O2::countValue()
{
    int val = 0;
    if (o2_get_counts(m_dev, &val) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + ": failed");

    return val;
}

float O2::voltageValue()
{
    float val = 0.0;
    if (o2_get_voltage(m_dev, &val) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + ": failed");

    return val;
}

float O2::o2Value()
{
    float val = 0.0;
    if (o2_get_value(m_dev, &val) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + ": failed");

    return val;
}
