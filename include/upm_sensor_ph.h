#ifndef _UPM_PH_H_
#define _UPM_PH_H_

#include <stdarg.h>
#include "upm.h"
#include "mraa/types.h"

/**
 * pH sensor value types
 */
typedef enum {
    UPM_PH_RAW = 0,
    UPM_PH_NORMALIZED,
    UPM_PH_PH
} upm_ph_value_t;


// pH sensor function table
typedef struct upm_ft_ph {
    void (*get_upm_descriptor) (upm_sensor_descriptor* desc);
    void* (*upm_ph_init) (int pin, float aref);
    upm_result_t (*upm_ph_close) (void* dev);
    upm_result_t (*upm_ph_set_offset) (void* dev, float ph_offset);
    upm_result_t (*upm_ph_get_value) (void* dev, float* value, upm_ph_value_t unit);
} upm_ft_ph;

// pH sensor helper methods
typedef upm_ft_ph (*func_get_upm_ft_ph)();

#endif /* _UPM_PH_H_ */
