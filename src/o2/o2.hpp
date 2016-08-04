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
#pragma once

#include <string>
#include <mraa/aio.h>
#include <o2.h>

namespace upm {
/**
 * @brief O2 Oxygen Gas Sensor library
 * @defgroup groveo2 libupm-groveo2
 * @ingroup seeed analog gaseous
 */

/**
 * @library groveo2
 * @sensor groveo2
 * @comname O2 Sensor
 * @type gaseous
 * @man seeed
 * @con analog
 *
 * @brief API for the O2 Oxygen Gas Sensor
 *
 * The O2 Oxygen Gas sensor measures the oxygen concentration in the air
 *
 * @image html groveo2.jpg
 * @snippet groveo2.cxx Interesting
 */
class O2 {
public:
    /**
     *  O2 Oxygen Gas sensor constructor
     *
     * @param pin Analog pin to use
     */
    O2(int pin);

    /**
     * O2 destructor
     */
    ~O2();

    /**
     * Set adc voltage reference
     *
     * @return Raw counts
     */
    void setAref(float aref);

    /**
     * Get adc voltage reference
     *
     * @return Current adc vref
     */
    float getAref();

    /**
     * Set raw adc count offset
     *
     * @return Raw counts
     */
    void setRawOffset(float raw_offset);

    /**
     * Get sensor offset
     *
     * @return Current sensor offset
     */
    float getRawOffset();

    /**
     * Set raw adc count offset
     *
     * @return Raw counts
     */
    void setRawScale(float raw_scale);

    /**
     * Get sensor scale
     *
     * @return Current sensor scale
     */
    float getRawScale();

    /**
     * Measures raw sensor counts from the sensor
     *
     * @return Raw counts
     */
    int countValue();

    /**
     * Measures voltage from the sensor
     *
     * @return Oxygen concentration as voltage
     */
    float voltageValue();

    /**
     * Measures O2 percent from the sensor
     *
     * @return Oxygen concentration (%)
     */
    float o2Value();

private:
    o2_context m_dev;
};
}
