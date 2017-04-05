//Modified: Abhishek Malik <abhishek.malik@intel.com>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "led.h"

#include "upm_utilities.h"
#include "mraa.h"

int main(void)
{
    if (mraa_init() != MRAA_SUCCESS)
    {
        fprintf(stderr,"Failed to initialize mraa\n");
        return -1;
    }

    printf("Before LED init\n");

    // CONFIG_BOARD_ARDUINO_101
    int onboard_led_pin = 13;
#if defined(CONFIG_BOARD_QUARK_D2000_CRB)
    onboard_led_pin = 9;
#elif defined(CONFIG_BOARD_QUARK_SE_C1000_DEVBOARD)
    onboard_led_pin = 64;      // pin 62 is also an onboard LED
#endif

    led_context dev = led_init(onboard_led_pin);
    printf("LED init\n");
    while(1){
        printf("Turning on\n");
        if(led_on(dev) != UPM_SUCCESS){
            printf("problem turning the LED on\n");
        }
        upm_delay_us(750000);
        printf("Turning off\n");
        if(led_off(dev) != UPM_SUCCESS){
            printf("problem turning the LED off\n");
        }
        upm_delay_us(750000);
    }
    led_close(dev);
    return 0;
}
