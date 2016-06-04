/*
 * Authors:
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
#ifndef UPM_UTILITIES_H_
#define UPM_UTILITIES_H_

inline const char * to_char(upm_sensor_category category)
{
    switch(category) {
        case UPM_PH:
            return "PH";
            break;
        case UPM_PRESSURE:
            return "PRESSURE";
            break;
        case UPM_SERVO:
            return "SERVO";
            break;
        case UPM_TEMPERATURE:
            return "TEMPERATURE";
            break;
        default:
            return "INVALID";
    }
}

static inline void upm_delay(int time){
#ifdef linux
	sleep(time);
#endif
}

static inline void upm_delay_ms(int time){
#ifdef linux
	usleep(1000 * time);
#endif
}

static inline void upm_delay_us(int time){
#ifdef linux
	usleep(time);
#endif
}

#endif /* UPM_UTILITIES_H_ */
