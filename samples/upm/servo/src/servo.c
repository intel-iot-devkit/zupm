//Modified: Abhishek Malik <abhishek.malik@intel.com>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "es08a.h"

#include "upm_utilities.h"
#include "mraa.h"

int main()
{
    if (mraa_init() != MRAA_SUCCESS)
    {
        fprintf(stderr,"Failed to initialize mraa\n");
        return -1;
    }

#if defined(CONFIG_BOARD_ARDUINO_101_SSS)
    es08a_context dev = es08a_init(3, ES08A_MIN_PULSE_WIDTH, ES08A_MAX_PULSE_WIDTH);
#elif defined(CONFIG_BOARD_QUARK_D2000_CRB)
    es08a_context dev = es08a_init(6, ES08A_MIN_PULSE_WIDTH, ES08A_MAX_PULSE_WIDTH);
#elif defined(CONFIG_BOARD_QUARK_SE_C1000_DEVBOARD) || defined(CONFIG_BOARD_QUARK_SE_C1000_DEVBOARD_SS)
    es08a_context dev = es08a_init(27, ES08A_MIN_PULSE_WIDTH, ES08A_MAX_PULSE_WIDTH);
#endif
    if(dev == NULL){
        printf("unable to initialize the servo context\n");
    }

    if(es08a_set_angle(dev, 90) != UPM_SUCCESS){
        printf("unable to set angle to 90 degrees\n");
    }
    upm_delay_us(1000000);

    if(es08a_set_angle(dev, 180) != UPM_SUCCESS){
        printf("unable to set angle to 180 degrees\n");
    }
    upm_delay_us(1000000);

    if(es08a_set_angle(dev, 90) != UPM_SUCCESS){
        printf("unable to set angle to 90 degrees\n");
    }
    upm_delay_us(1000000);

    es08a_halt(dev);

    return 0;
}
