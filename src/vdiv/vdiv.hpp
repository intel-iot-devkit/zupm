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
#pragma once

#include <iostream>
#include <string>

#include "mraa/aio.hpp"
#include "vdiv.h"

namespace upm {
  /**
   * @brief DFRobot volts sensors
   * @defgroup vdiv libupm-vdiv
   * @ingroup dfrobot liquid analog
   */

  /**
   * @library vdiv
   * @sensor vdiv
   * @comname DFRobot volts Sensors
   * @type liquid
   * @man dfrobot 
   * @web http://www.dfrobot.com/index.voltagep?route=product/product&product_id=1110
   * @con analog
   *
   * @brief API for the DFRobot volts Sensors
   *
   * This sensor family returns an analog voltage proportional to the
   * acidity or alkalinity of a liquid -- it's volts value.
   *
   * This driver was developed using the DFRobot Analog volts meter and
   * the DFRobot Analog volts Meter Pro.
   *
   *
   * Calibration instructions, taken and slightly reworded from the
   *  DFRobot wiki at:
   *  http://dfrobot.com/wiki/index.voltagep/PH_meter%28SKU:_SEN0161%29
   *
   *  1) Connect equipment: the volts electrode is connected to the BNC
   *  connector on the volts meter board, and then the volts meter board is
   *  connected to the analog port 0 of the controller. When the
   *  controller gets power, you will see the blue LED on board is on.
   *
   *  2) Put the volts electrode into the standard solution whose volts
   *  value is 7.00.  Run the vdiv example and note the volts output
   *  value.  Compare the value with 7.00, and calculate the
   *  difference.  This is the value you should supply to the
   *  setOffset() method.
   *
   *  3) Put the volts electrode into the volts standard solution whose
   *  value is 4.00. Then wait about one minute, and adjust the
   *  potentiometer on the interface board.  Let the value stabilise
   *  at around 4.00. At this time,the acidic calibration has been
   *  completed and you can measure the volts value of an acidic
   *  solution.
   *
   *  4) According to the linear characteristics of volts electrode
   *  itself, after the above calibration,you can directly measure the
   *  volts value of the alkaline solution. If you want to get better
   *  accuracy, you can recalibrate it. Alkaline calibration use the
   *  standard solution whose volts value is 9.18.  Also adjust the
   *  potentiometer and let the value stabilise at around 9.18. After
   *  this calibration, you can measure the volts value of an alkaline
   *  solution.
   *
   * @image html vdiv.jpg
   * @snippet vdiv.cxx Interesting
   */

  class VDIV {
  public:

    /**
     * VDIV constructor
     *
     * @param pin Analog pin to use
     * @param vref Analog reference voltage; default is 5.0 V
     */
    VDIV(int pin, float vref = 5.0);

    /**
     * VDIV destructor
     */
    ~VDIV();

    /**
     * Returns the voltage detected on the analog pin
     *
     * @return The detected voltage
     */
    float volts();

    /**
     * Specifies the offset determined from calibration.  The default
     * is 0.0.
     *
     * @param offset The offset value to use
     */
    void setOffset(float offset);

    /**
     * Specifies the scale determined from calibration.  The default
     * is 1.0.
     *
     * @param scale The scale value to use
     */
    void setScale(float scale);

  private:
    void* _dev;
  };
}


