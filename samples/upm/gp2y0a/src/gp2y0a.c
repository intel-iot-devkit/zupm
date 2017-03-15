//Modified: Abhishek Malik <abhishek.malik@intel.com>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "gp2y0a.h"

#include "upm_utilities.h"
#include "mraa.h"

int main()
{
    if (mraa_init() != MRAA_SUCCESS)
    {
        fprintf(stderr,"Failed to initialize mraa\n");
        return -1;
    }

    gp2y0a_context dev = gp2y0a_init(0, 5.0);
    float val;
    while(1){
        if(gp2y0a_get_value(dev, 5.0, 20, &val) != UPM_SUCCESS){
            printf("Failed to get any values from the sensor\n");
        }
        
#if defined(CONFIG_BOARD_QUARK_D2000_CRB)
    printf("Moisture Value: %d mv\n", (int)(val*1000));
#elif defined(CONFIG_BOARD_ARDUINO_101_SSS)
    printf("Moisture Value: %f v\n", val);
#endif
        upm_delay(1);
    }
    gp2y0a_close(dev);

    return 0;
}

