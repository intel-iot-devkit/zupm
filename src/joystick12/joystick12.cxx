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

#include "joystick12.hpp"

using namespace upm;

JOYSTICK12::JOYSTICK12(int ai_x, int ai_y, float vref) :
    _dev(upm_joystick12_init(ai_x, ai_y)) {}

JOYSTICK12::~JOYSTICK12()
{
    upm_joystick12_close(_dev);
}

void JOYSTICK12::setOffset_x(float offset)
{
    upm_joystick12_set_offset_x(_dev, offset);
}

void JOYSTICK12::setOffset_y(float offset)
{
    upm_joystick12_set_offset_y(_dev, offset);
}

void JOYSTICK12::setScale_x(float scale)
{
    upm_joystick12_set_scale_x(_dev, scale);
}

void JOYSTICK12::setScale_y(float scale)
{
    upm_joystick12_set_scale_y(_dev, scale);
}

float JOYSTICK12::getXInput()
{
    float value;
    upm_joystick12_get_value_x(_dev, &value);
    return value;
}

float JOYSTICK12::getYInput()
{
    float value;
    upm_joystick12_get_value_y(_dev, &value);
    return value;
}

void JOYSTICK12::zero()
{
    upm_joystick12_zero(_dev);
}

void JOYSTICK12::calibrate_x()
{
    upm_joystick12_calibrate_x(_dev);
}

void JOYSTICK12::calibrate_y()
{
    upm_joystick12_calibrate_y(_dev);
}
