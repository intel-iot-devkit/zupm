//Modified: Abhishek Malik <abhishek.malik@intel.com>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "loudness.h"

#include "upm_utilities.h"
#include "mraa.h"

int main()
{
    if (mraa_init() != MRAA_SUCCESS)
    {
        fprintf(stderr,"Failed to initialize mraa\n");
        return -1;
    }

    // Use analog pin 0
    loudness_context dev = loudness_init(0);
    if (!dev)
    {
        fprintf(stderr,"Failed to initialize loudness sensor\n");
        return -1;
    }

    int val;
    while(1){
        if(loudness_get_value(dev, &val) != UPM_SUCCESS){
            printf("Failed to get any values from the sensor\n");
        }
        printf("Loudness Value: %d\n", val);
        upm_delay(1);
    }
    loudness_close(dev);

    return 0;
}
