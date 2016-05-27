#include <stdio.h>
#include <stdlib.h>
#include "ad8232.h"

struct _upm_ad8232 {
	mraa_aio_context		aio;
	mraa_gpio_context		gpio_lo_plus;
	mraa_gpio_context		gpio_lo_minus;
	uint8_t					aio_pin;
	uint8_t					gpio_lo_plus_pin;
	uint8_t					gpio_lo_minus_pin;
	int						a_res;
};


void* upm_get_function_table(upm_sensor_types sensor_type){
	if(sensor_type == UPM_HEART_RATE){
		struct _upm_hr_ft *ft = malloc(sizeof(*ft));
		ft->upm_heart_rate_sensor_init = upm_ad8232_init;
		ft->upm_heart_rate_sensor_close = upm_ad8232_close;
		ft->upm_heart_rate_sensor_get_heart_rate = upm_ad8232_get_value;
		return ft;
	}
	return NULL;
}


void* upm_ad8232_init(int num,...){
	upm_ad8232 dev = (upm_ad8232) malloc(sizeof(struct _upm_ad8232));
	if(dev == NULL){
		printf("Unable to assign memory to the Heart Rate Monitor structure");
		return NULL;
	}

	va_list pin_list;

	va_start(pin_list, num);
	dev->gpio_lo_plus_pin = va_arg(pin_list, int);
	dev->gpio_lo_minus_pin = va_arg(pin_list, int);
	dev->aio_pin = va_arg(pin_list, int);

	dev->aio = mraa_aio_init(dev->aio_pin);
	dev->gpio_lo_plus = mraa_gpio_init(dev->gpio_lo_plus_pin);
	dev->gpio_lo_minus = mraa_gpio_init(dev->gpio_lo_minus_pin);

	if(dev->aio == NULL || dev->gpio_lo_minus == NULL || dev->gpio_lo_plus == NULL){
		printf("The pins did not initialize correctly");
		return NULL;
	}

	if(mraa_gpio_dir(dev->gpio_lo_minus, MRAA_GPIO_IN) != MRAA_SUCCESS || mraa_gpio_dir(dev->gpio_lo_plus, MRAA_GPIO_IN) != MRAA_SUCCESS){
		printf("Couldn't set the direction for the GPIO pins");
		return NULL;
	}

	dev->a_res = (1 << mraa_aio_get_bit(dev->aio));
	return dev;
}

upm_result_t upm_ad8232_close(void* dev){
	upm_ad8232 device = (upm_ad8232) dev;
	if(device->gpio_lo_minus != NULL)
		mraa_gpio_close(device->gpio_lo_minus);
	if(device->gpio_lo_plus != NULL)
		mraa_gpio_close(device->gpio_lo_plus);
	if(device->aio != NULL)
		mraa_aio_close(device->aio);
	if(device != NULL)
		free(device);
	return UPM_SUCCESS;
}

upm_result_t upm_ad8232_get_value(void* dev, float* value, upm_heart_rate_units rate_unit){
	upm_ad8232 device = (upm_ad8232) dev;
	float reading;
	if(mraa_gpio_read(device->gpio_lo_minus) || mraa_gpio_read(device->gpio_lo_plus)){
		reading = 0;
	}
	else{
		reading = mraa_aio_read(device->aio);
	}

	if(rate_unit == RAW_HR){
		*value = reading;
	}
	return UPM_SUCCESS;
}
