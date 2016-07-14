/*
 * Author: Nandkishor Sonar <Nandkishor.Sonar@intel.com>,
 *         Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * LIGHT-TO-DIGITAL CONVERTER [TAOS-TSL2561]
 *   Inspiration and lux calculation formulas from data sheet
 *   URL: http://www.adafruit.com/datasheets/TSL2561.pdf
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

#include "tsl2561.h"

struct _upm_tsl2561 {
    mraa_i2c_context    i2c;
    int                 bus;
    uint8_t             address;
    uint8_t             gain;
    uint8_t             integration_time;
};

const char upm_tsl2561_name[] = "TSL2561";
const char upm_tsl2561_description[] = "Grove Digital Light Sensor";
const upm_protocol_t upm_tsl2561_protocol[] = {UPM_I2C};
const upm_sensor_t upm_tsl2561_category[] = {UPM_LIGHT};

const upm_sensor_descriptor_t upm_tsl2561_get_descriptor (){
    upm_sensor_descriptor_t usd;
    usd.name = upm_tsl2561_name;
    usd.description = upm_tsl2561_description;
    usd.protocol_size = 1;
    usd.protocol = upm_tsl2561_protocol;
    usd.category_size = 1;
    usd.category = upm_tsl2561_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_tsl2561_init_name,
    .upm_sensor_close = &upm_tsl2561_close,
    .upm_sensor_get_descriptor = &upm_tsl2561_get_descriptor
};

static const upm_light_ft lft =
{
    .upm_light_get_value = &upm_tsl2561_get_lux
};

const void* (*upm_get_ft) (upm_sensor_t sensor_type) = &upm_tsl2561_get_ft;

// forward declaration
upm_result_t upm_tsl2561_compute_lux(const void* dev, int *int_data);

const void* upm_tsl2561_get_ft(upm_sensor_t sensor_type){
    if(sensor_type == UPM_LIGHT){
        return &lft;
    }
    else if(sensor_type == UPM_SENSOR){
        return &ft;
    }
    return NULL;
}

void* upm_tsl2561_init_name(){
    return NULL;
}

void* upm_tsl2561_init(int bus, uint8_t dev_address, uint8_t gain, uint8_t integration_time){
    upm_tsl2561 dev = (upm_tsl2561) malloc(sizeof(struct _upm_tsl2561));

    dev->bus = bus;
    dev->address = dev_address;
    dev->gain = gain;
    dev->integration_time = integration_time;

    dev->i2c = mraa_i2c_init(dev->bus);
    if(dev->i2c == NULL){
        printf("The i2c context could not be initialized\n");
        return NULL;
    }

    mraa_i2c_address(dev->i2c, dev->address);

    // POWER UP.
    if(mraa_i2c_write_byte_data(dev->i2c, CONTROL_POWERON, REGISTER_Control) != MRAA_SUCCESS){
        printf("Unable to power up tsl2561\n");
        return NULL;
    }

    // Power on Settling time
    upm_delay_us(1000);

    // Gain & Integration time.
    if(mraa_i2c_write_byte_data(dev->i2c, (dev->gain | dev->integration_time), REGISTER_Timing) != MRAA_SUCCESS){
        printf("Unable to set gain/time\n");
        return NULL;
    }

    // Set interrupt threshold to default.
    if(mraa_i2c_write_byte_data(dev->i2c, 0x00, REGISTER_Interrupt) != MRAA_SUCCESS){
        printf("Unable to set gain/time\n");
        return NULL;
    }
    printf("Bus number and something: %d\n", dev->bus);

    return dev;
}

void upm_tsl2561_close(void* dev){
    upm_tsl2561 device = (upm_tsl2561) dev;
    if(mraa_i2c_write_byte_data(device->i2c, CONTROL_POWEROFF, REGISTER_Control) != MRAA_SUCCESS){
        printf("Unable turn off device\n");
    }
    free(device);
}

upm_result_t upm_tsl2561_get_lux(const void* dev, float* lux){
    upm_tsl2561 device = (upm_tsl2561) dev;
    int lux_val=0;

    upm_tsl2561_compute_lux(device, &lux_val);

    *lux = (float) lux_val;
    return UPM_SUCCESS;
}

upm_result_t upm_tsl2561_compute_lux(const void* dev, int *int_data) {
    upm_tsl2561 device = (upm_tsl2561) dev;

    int lux;
    uint16_t raw_lux_ch_0;
    uint16_t raw_lux_ch_1;
    uint8_t ch0_low, ch0_high, ch1_low, ch1_high;

    if(upm_tsl2561_i2c_read_reg(device, REGISTER_Channal0L, &ch0_low) != UPM_SUCCESS){
        printf("Unable to read channel0L in getRawLux()\n");
        return UPM_ERROR_OPERATION_FAILED;
    }

    if(upm_tsl2561_i2c_read_reg(device, REGISTER_Channal0H, &ch0_high) != UPM_SUCCESS){
        printf("Unable to read channel0L in getRawLux()\n");
        return UPM_ERROR_OPERATION_FAILED;
    }
    printf("ch0_low: %d, ch0_high: %d, ch1_low: %d, ch1_high: %d\n", ch0_low, ch0_high, ch1_low, ch1_high);
    raw_lux_ch_0 = ch0_high*256 + ch0_low;

    if(upm_tsl2561_i2c_read_reg(device, REGISTER_Channal1L, &ch1_low) != UPM_SUCCESS){
        printf("Unable to read channel0L in getRawLux()\n");
        return UPM_ERROR_OPERATION_FAILED;
    }

    if(upm_tsl2561_i2c_read_reg(device, REGISTER_Channal1H, &ch1_high) != UPM_SUCCESS){
        printf("Unable to read channel0L in getRawLux()\n");
        return UPM_ERROR_OPERATION_FAILED;
    }
    raw_lux_ch_1 = ch1_high*256 + ch1_low;

    uint64_t scale = 0;

    switch(device->integration_time){
        case 0: // 13.7 msec
            scale = LUX_CHSCALE_TINT0;
            break;
        case 1: // 101 msec
            scale = LUX_CHSCALE_TINT1;
            break;
        case 2: // assume no scaling
            scale = (1 << LUX_CHSCALE);
            break;
    }

    // scale if gain is NOT 16X
    if(!device->gain)
        scale = scale << 4;

    uint64_t channel1 = 0;
    uint64_t channel0 = 0;

    // scale the channel values
    channel0 = (raw_lux_ch_0 * scale) >> LUX_CHSCALE;
    channel1 = (raw_lux_ch_1 * scale) >> LUX_CHSCALE;

    // find the ratio of the channel values (Channel1/Channel0)
    // protect against divide by zero
    uint64_t ratio_1 = 0;
    if (channel0 != 0)
        ratio_1 = (channel1 << (LUX_RATIOSCALE+1)) / channel0;

    // round the ratio value
    unsigned long ratio = (ratio_1 + 1) >> 1;
    unsigned int b, m;

    // CS package
    // Check if ratio <= eachBreak ?
    if ((ratio >= 0) && (ratio <= LUX_K1C)){
        b=LUX_B1C; m=LUX_M1C;
    }
    else if (ratio <= LUX_K2C){
        b=LUX_B2C; m=LUX_M2C;
    }
    else if (ratio <= LUX_K3C){
        b=LUX_B3C; m=LUX_M3C;
    }
    else if (ratio <= LUX_K4C){
        b=LUX_B4C; m=LUX_M4C;
    }
    else if (ratio <= LUX_K5C){
        b=LUX_B5C; m=LUX_M5C;
    }
    else if (ratio <= LUX_K6C){
        b=LUX_B6C; m=LUX_M6C;
    }
    else if (ratio <= LUX_K7C){
        b=LUX_B7C; m=LUX_M7C;
    }
    else if (ratio > LUX_K8C){
        b=LUX_B8C; m=LUX_M8C;
    }
    uint64_t temp_lux = 0;
    temp_lux = ((channel0 * b) - (channel1 * m));
    // do not allow negative lux value
    if (temp_lux < 0) temp_lux = 0;

    // round lsb (2^(LUX_SCALE-1))
    temp_lux += (1 << (LUX_SCALE-1));

    // strip off fractional portion
    lux = temp_lux >> LUX_SCALE;

    *int_data = lux;

    return UPM_SUCCESS;
}

upm_result_t upm_tsl2561_i2c_write_reg(void* dev, uint8_t reg, uint8_t value){
    upm_tsl2561 device = (upm_tsl2561) dev;
    // Start transmission to device
    if(mraa_i2c_address(device->i2c, device->address) != MRAA_SUCCESS){
        printf("Failure in setting i2c address\n");
        return UPM_ERROR_OPERATION_FAILED;
    }

    // Write register to I2C
    if(mraa_i2c_write_byte(device->i2c, reg) != MRAA_SUCCESS){
        printf("Unable to write address on i2c bus\n");
        return UPM_ERROR_OPERATION_FAILED;
    }

    // Write value to I2C
    if(mraa_i2c_write_byte(device->i2c, value) != MRAA_SUCCESS){
        printf("Unable to write data on i2c bus\n");
        return UPM_ERROR_OPERATION_FAILED;
    }

    upm_delay_ms(100000);
    return UPM_SUCCESS;
}

upm_result_t upm_tsl2561_i2c_read_reg(void* dev, uint8_t reg, uint8_t* data){
   upm_tsl2561 device = (upm_tsl2561) dev;
   // Start transmission to device
    if(mraa_i2c_address(device->i2c, device->address) != MRAA_SUCCESS){
        printf("Failure in setting i2c address\n");
        return UPM_ERROR_OPERATION_FAILED;
    }

    // Send address of register to be read.
    if(mraa_i2c_write_byte(device->i2c, reg) != MRAA_SUCCESS){
        printf("Unable to write address on i2c bus\n");
        return UPM_ERROR_OPERATION_FAILED;
    }

    // Read byte.
    *data = mraa_i2c_read_byte(device->i2c);

    //printf("read some value from the sensor: %d\n", *data);
    upm_delay_ms(100);
    return UPM_SUCCESS;
}
