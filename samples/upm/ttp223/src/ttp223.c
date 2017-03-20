//Modified: Abhishek Malik <abhishek.malik@intel.com>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "ttp223.h"

#include "upm_utilities.h"
#include "mraa.h"

int main()
{
    if (mraa_init() != MRAA_SUCCESS)
    {
        fprintf(stderr,"Failed to initialize mraa\n");
        return -1;
    }

    ttp223_context dev = ttp223_init(2);
    bool abc = 0;
    while(1){
        if(ttp223_is_pressed(dev, &abc) != UPM_SUCCESS){
            printf("an error has occurred\n");
        }
        upm_delay(1);
        printf("value retrieved: %d\n", abc);
    }

    return 0;
}
