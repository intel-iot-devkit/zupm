//Modified: Abhishek Malik <abhishek.malik@intel.com>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "tsl2561.h"

#include "upm_utilities.h"
#include "mraa.h"

int main()
{
    if (mraa_init() != MRAA_SUCCESS)
    {
        fprintf(stderr,"Failed to initialize mraa\n");
        return -1;
    }

    tsl2561_context dev = tsl2561_init(0, TSL2561_Address, GAIN_0X, INTEGRATION_TIME1_101MS);
    if (!dev)
    {
        fprintf(stderr,"Failed to initialize digital light sensor\n");
        return -1;
    }

    float abc = 0;
    while (1)
    {
        if(tsl2561_get_lux(dev, &abc) != UPM_SUCCESS){
            fprintf(stderr, "ERROR !! ERROR !! ERROR!!");
            return -1;
        }

#if defined(CONFIG_BOARD_QUARK_D2000_CRB)
        int abc_int = abc;
        printf("value retrieved: %d\n", abc_int);
#else
        printf("value retrieved: %f\n", abc);
#endif
        upm_delay_ms(500);
    }

    return 0;
}
