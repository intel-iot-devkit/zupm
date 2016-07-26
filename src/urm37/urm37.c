/*
 * Author: Jon Trulson <jtrulson@ics.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * Thanks to Adafruit for supplying a google translated version of the
 * Chinese datasheet and some clues in their code.
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

#include "urm37.h"
#include "mraa/aio.h"
#include "mraa/uart.h"

typedef struct _urm37_context {
    mraa_aio_context    aio;
    mraa_gpio_context   gpio_reset_pin;
    mraa_gpio_context   gpio_trig_pin;
    mraa_uart_context   uart;
    uint8_t             analog_pin;
    uint8_t             reset_pin;
    uint8_t             trigger_pin;
    uint8_t             uart_pin;
    bool                is_analog_mode;
    int                 a_res;
    int                 degrees;
} *urm37_context;

urm37_context urm37_init(uint8_t a_pin, uint8_t reset_pin,
                         uint8_t trigger_pin, float a_ref,
                         uint8_t uart, bool analog_mode) {

    urm37_context dev = (urm37_context)malloc(sizeof(struct _urm37_context));

    dev->analog_pin =  a_pin;
    dev->uart_pin = uart;
    dev->reset_pin = reset_pin;
    dev->trigger_pin = trigger_pin;
    // read out the mode as well to start off with
    dev->is_analog_mode = analog_mode;

    // initialize the contexts
    // analog part
    dev->aio = mraa_aio_init(dev->analog_pin);
    // both analog and uart
    dev->gpio_reset_pin = mraa_gpio_init(dev->reset_pin);
    dev->gpio_trig_pin = mraa_gpio_init(dev->trigger_pin);
    // uart part
    dev->uart = mraa_uart_init(dev->uart_pin);

    // setting degrees (this needs to be verified)
    dev->degrees = 0;

    // only analog
    dev->a_res = (1 << mraa_aio_get_bit(dev->aio));
    mraa_gpio_dir(dev->gpio_trig_pin, MRAA_GPIO_OUT);
    mraa_gpio_write(dev->gpio_trig_pin, 1);

    // uart only
    // 9600 baud is the only supported baud rate...
    mraa_uart_set_baudrate(dev->uart, 9600);
    mraa_uart_set_non_blocking(dev->uart, false);

    // analog and uart
    mraa_gpio_dir(dev->gpio_reset_pin, MRAA_GPIO_OUT);
    urm37_reset(dev);

    return dev;
}

void urm37_close(urm37_context dev){
    if (dev->aio)
        free(dev->aio);
    if (dev->gpio_reset_pin)
        free(dev->gpio_reset_pin);
    if (dev->gpio_trig_pin)
        free(dev->gpio_trig_pin);
    if (dev->uart)
        free(dev->uart);
    free(dev);
}

upm_result_t upm_urm37_reset(urm37_context dev) {
    mraa_gpio_write(dev->gpio_reset_pin, 0);
    // OS specific : Add to utilities
    usleep(100);
    mraa_gpio_write(dev->gpio_reset_pin, 1);
    // OS specific
    // wait for reset to complete
    sleep(3);

    return UPM_SUCCESS;
}

bool urm37_data_available(urm37_context dev, uint32_t millis){
    if (mraa_uart_data_available(dev->uart, millis))
        return true;
    else
        return false;
}

upm_result_t urm37_read_data_string(urm37_context dev, uint32_t len,
                                    char* data){
    int v = mraa_uart_read(dev->uart, data, (size_t) len);
    printf("success/failure: %d\n", v);
    return UPM_SUCCESS;
}

upm_result_t urm37_write_data_string(urm37_context dev, const char* data_w,
                                     int len) {
    mraa_uart_flush(dev->uart);
    //printf("send command: %c %c %c %c\n", data_w[0], data_w[1], data_w[2], data_w[3]);
    mraa_uart_write(dev->uart, data_w, len);
    return UPM_SUCCESS;
}

upm_result_t urm37_send_command(urm37_context dev, char* cmd,
                                char* response, int len) {
    if (dev->is_analog_mode) {
        printf("UART commands are not available in analog mode\n");
        return UPM_ERROR_NOT_SUPPORTED;
    }

    int tries = 0;
    //char resp[8];

    while (tries++ < URM37_MAX_RETRIES) {
        urm37_write_data_string(dev, cmd, len);
        //mraa_boolean_t ret = FALSE;
        if (!urm37_data_available(dev, URM37_WAIT_TIMEOUT)){
            printf("Time out occurred\n");
            continue;
        }
        /*upm_urm37_data_available(device, URM37_WAIT_TIMEOUT, &ret);
          if (ret != TRUE){
          printf("Time out occured\n");
          return UPM_ERROR_TIMED_OUT;
          }*/

        // assigning array some space
        int length = 8;

        char resp[length];
        // JET - why print out stack data??
        printf("prior: %d %d %d %d %d %d %d %d \n", resp[0],resp[1],resp[2],resp[3],resp[4],resp[5],resp[6],resp[7]);
        sleep(1);
        //        char* resp = (char*) malloc(sizeof(char) * length);

// JET - wrong pointer type!        urm37_read_data_string(dev->uart, length, resp);
        urm37_read_data_string(dev, length, resp);

        printf("returned value: %d %d %d %d %d %d %d %d \n", resp[0],resp[1],resp[2],resp[3],resp[4],resp[5],resp[6],resp[7]);
        if (resp[4] == '\0'){
            printf("Invalid Packet size\n");
            continue;
        }
        else{
            // we have data, verify cksum, return the response if it's
            // good, retry otherwise
            uint8_t cksum = (uint8_t)(resp[0] + resp[1] + resp[2]);

            if ((uint8_t)resp[3] != cksum){
                printf("The checksum failed\n");
                continue;
            }
            *response = *resp;
        }
    }
    return UPM_SUCCESS;
}

upm_result_t urm37_get_distance(urm37_context dev, float* distance) {

    if (dev->is_analog_mode) {
        // analog mode
        int val;

        mraa_gpio_write(dev->gpio_trig_pin, 0);
        val = mraa_aio_read(dev->aio);
        mraa_gpio_write(dev->gpio_trig_pin, 1);

        // convert to mV
        float volts = ((float)val * (A_REF / dev->a_res)) * 1000.0;

        // 6.8 mV/cm
        *distance = volts/6.8;

        return UPM_SUCCESS;
    }

    // UART mode
    // query distance cmd sequence
    int degrees = dev->degrees;
    uint8_t deg = (uint8_t)(degrees / 6);
    if (deg > 46){
        printf("Degrees out of range, must be between 0-270\n");
        return UPM_ERROR_OUT_OF_RANGE;
    }
    
    // not sure about this string related part
    uint8_t cksum = 0x22 + deg + 0x00;
    char cmd[URM37_MAX_CMD_LEN];
    cmd[0] = 0x22;
    cmd[1] = deg;
    cmd[2] = 0x00;
    cmd[3] = cksum;
    
    char response[URM37_MAX_RESP_LEN];
    urm37_send_command(dev, cmd, response, URM37_MAX_CMD_LEN);
    if (response[1] == '\0' || response[2] == '\0'){
        printf("Send Command failed\n");
        return UPM_ERROR_OPERATION_FAILED;
    }
    
    uint8_t h = (uint8_t) response[1];
    uint8_t l = (uint8_t) response[2];
    
    *distance = (float)((h << 8) | l);

    return UPM_SUCCESS;
}

upm_result_t urm37_get_temperature(urm37_context dev, float* temperature){
    if (dev->is_analog_mode) {
        printf("Temperature measurement not available in analog mode\n");
        return UPM_ERROR_NOT_SUPPORTED;
    }

    // UART mode
    char cmd[URM37_MAX_CMD_LEN];
    cmd[0] = 0x11;
    cmd[1] = 0x00;
    cmd[2] = 0x00;
    cmd[3] = 0x11;
    
    char response[URM37_MAX_RESP_LEN];
    urm37_send_command(dev, cmd, response, URM37_MAX_CMD_LEN);
    
    if (response[1] == '\0' || response[2] == '\0'){
        printf("Send Command failed\n");
        return UPM_ERROR_OPERATION_FAILED;
    }
    
    uint8_t h = (uint8_t) response[1];
    uint8_t l = (uint8_t) response[2];
    
    *temperature = (float)((h & 0x0f) * 256 + l) / 10.0;
    if (h & 0xf0)
        *temperature *= -1;

    return UPM_SUCCESS;
}

upm_result_t urm37_read_EEPROM(urm37_context dev, uint8_t addr, uint8_t* value){
    if (dev->is_analog_mode) {
        printf("EEPROM not available in analog mode\n");
        return UPM_ERROR_NOT_SUPPORTED;
    }

    if (addr > 0x04){
        printf("Address must be between 0x00-0x04");
        return UPM_ERROR_OUT_OF_RANGE;
    }

    uint8_t cksum = 0x33 + addr + 0x00;
    char cmd[URM37_MAX_CMD_LEN];
    cmd[0] = 0x33;
    cmd[1] = addr;
    cmd[2] = 0x00;
    cmd[3] = cksum;

    char response[URM37_MAX_RESP_LEN];
    urm37_send_command(dev, cmd, response, URM37_MAX_CMD_LEN);

    if (response[1] == '\0' || response[2] == '\0'){
        printf("Send Command failed\n");
        return UPM_ERROR_OPERATION_FAILED;
    }

    *value = response[2];
    return UPM_SUCCESS;
}

upm_result_t urm37_write_EEPROM(urm37_context dev, uint8_t addr, uint8_t value){
    if (dev->is_analog_mode) {
        printf("EEPROM not available in analog mode\n");
        return UPM_ERROR_NOT_SUPPORTED;
    }

    if (addr > 0x04){
        printf("Address must be between 0x00-0x04");
        return UPM_ERROR_OUT_OF_RANGE;
    }

    uint8_t cksum = 0x44 + addr + value;
    char cmd[URM37_MAX_CMD_LEN];
    cmd[0] = 0x44;
    cmd[1] = addr;
    cmd[2] = value;
    cmd[3] = cksum;

    char response[URM37_MAX_RESP_LEN];
    urm37_send_command(dev, cmd, response, URM37_MAX_CMD_LEN);

    if (response[1] == '\0' || response[2] == '\0'){
        printf("Send Command failed\n");
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

upm_result_t urm37_get_degrees(urm37_context dev, int* degrees){
    *degrees = dev->degrees;
    return UPM_SUCCESS;
}

upm_result_t urm37_set_degrees(urm37_context dev, int degrees){
    dev->degrees = degrees;
    return UPM_SUCCESS;
}
