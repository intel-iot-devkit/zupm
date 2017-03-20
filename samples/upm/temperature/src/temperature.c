//Modified: Abhishek Malik <abhishek.malik@intel.com>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "temperature.h"

#include "upm_utilities.h"
#include "mraa.h"

int main()
{
    if (mraa_init() != MRAA_SUCCESS)
    {
        fprintf(stderr,"Failed to initialize mraa\n");
        return -1;
    }

    temperature_context dev = temperature_init(0);
    float val;
    while(1){
        if(temperature_get_value(dev, &val) != UPM_SUCCESS){
            printf("Failed to get any values from the sensor\n");
        }
#if defined(CONFIG_BOARD_QUARK_D2000_CRB)
        printf("Temperature Value: %d\n", (int)val);
#elif defined(CONFIG_BOARD_ARDUINO_101_SSS)
        printf("Temperature Value: %f\n", val);
#endif

        upm_delay(1);
    }
    temperature_close(dev);

    return 0;
}

