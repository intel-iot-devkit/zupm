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

#include "joystick12.h"

namespace upm {
  /**
   * @brief DFRobot normalized sensors
   * @defgroup joystick12 libupm-joystick12
   * @ingroup dfrobot liquid analog
   */

  /**
   * @library joystick12
   * @sensor joystick12
   * @comname DFRobot normalized Sensors
   * @type liquid
   * @man dfrobot 
   * @web http://www.dfrobot.com/index.joystickp?route=product/product&product_id=1110
   * @con analog
   *
   * @brief API for the DFRobot normalized Sensors
   *
   * This sensor family returns an analog voltage proportional to the
   * acidity or alkalinity of a liquid -- it's normalized value.
   *
   * This driver was developed using the DFRobot Analog normalized meter and
   * the DFRobot Analog normalized Meter Pro.
   *
   *
   * Calibration instructions, taken and slightly reworded from the
   *  DFRobot wiki at:
   *  http://dfrobot.com/wiki/index.joystickp/PH_meter%28SKU:_SEN0161%29
   *
   *  1) Connect equipment: the normalized electrode is connected to the BNC
   *  connector on the normalized meter board, and then the normalized meter board is
   *  connected to the analog port 0 of the controller. When the
   *  controller gets power, you will see the blue LED on board is on.
   *
   *  2) Put the normalized electrode into the standard solution whose normalized
   *  value is 7.00.  Run the joystick12 example and note the normalized output
   *  value.  Compare the value with 7.00, and calculate the
   *  difference.  This is the value you should supply to the
   *  setOffset() method.
   *
   *  3) Put the normalized electrode into the normalized standard solution whose
   *  value is 4.00. Then wait about one minute, and adjust the
   *  potentiometer on the interface board.  Let the value stabilise
   *  at around 4.00. At this time,the acidic calibration has been
   *  completed and you can measure the normalized value of an acidic
   *  solution.
   *
   *  4) According to the linear characteristics of normalized electrode
   *  itself, after the above calibration,you can directly measure the
   *  normalized value of the alkaline solution. If you want to get better
   *  accuracy, you can recalibrate it. Alkaline calibration use the
   *  standard solution whose normalized value is 9.18.  Also adjust the
   *  potentiometer and let the value stabilise at around 9.18. After
   *  this calibration, you can measure the normalized value of an alkaline
   *  solution.
   *
   * @image html joystick12.jpg
   * @snippet joystick12.cxx Interesting
   */

  class JOYSTICK12 {
  public:

    /**
     * JOYSTICK12 constructor
     *
     * @param ai_x Analog pin to use as x axis
     * @param ai_y Analog pin to use as y axis
     * @param vref Analog reference voltage; default is 5.0 V
     */
    JOYSTICK12(int ai_x, int ai_y, float vref = 5.0);

    /**
     * JOYSTICK12 destructor
     */
    ~JOYSTICK12();

    /**
     * Specifies the offset determined from calibration.  The default
     * is 0.0.
     *
     * @param offset The offset value to use
     */
    void setOffset_x(float offset);

    /**
     * Specifies the offset determined from calibration.  The default
     * is 0.0.
     *
     * @param offset The offset value to use
     */
    void setOffset_y(float offset);

    /**
     * Specifies the scale determined from calibration.  The default
     * is 1.0.
     *
     * @param scale The scale value to use
     */
    void setScale_x(float scale);

    /**
     * Specifies the scale determined from calibration.  The default
     * is 1.0.
     *
     * @param scale The scale value to use
     */
    void setScale_y(float scale);

    /**
     * Gets the X input
     * @return float X value, ranging from -1 to 1; 0 is mid
     */
    float getXInput();

    /**
     * Gets the Y input
     *
     * @return float Y value, ranging from -1 to 1; 0 is mid
     */
    float getYInput();

    /**
     * Zero the x and y axis
     *
     */
    void zero();

    /**
     * Set the x axis scale
     *
     */
    void calibrateX();

    /**
     * Set the y axis scale
     *
     */
    void calibrateY();

  private:
    joystick12_context _dev;
  };
}


