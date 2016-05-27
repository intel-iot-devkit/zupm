#include "ttp223.h"

struct _upm_ttp223 {
	mraa_gpio_context		gpio;
	uint8_t					gpio_pin;
	upm_boolean_t			isr_installed;
};

void* upm_get_function_table(upm_sensor_types sensor_type){
	if(sensor_type == UPM_TOUCH){
		struct _upm_touch_ft *ft = malloc(sizeof(*ft));
		ft->upm_touch_init = upm_ttp223_init;
		ft->upm_touch_close = upm_ttp223_close;
		ft->upm_touch_get_is_pressed = upm_ttp223_is_pressed;
		return ft;
	}
	return NULL;
}

void* upm_ttp223_init(int num,...){
	upm_ttp223 dev = (upm_ttp223) malloc(sizeof(struct _upm_ttp223));
	if(dev == NULL){
		printf("Unable to allocate space for the sensor struct\n");
		return NULL;
	}

	va_list pin_list;
	va_start(pin_list, num);
	dev->gpio_pin = va_arg(pin_list, int);
	va_end(pin_list);

	dev->gpio = mraa_gpio_init(dev->gpio_pin);
	mraa_gpio_dir(dev->gpio, MRAA_GPIO_IN);
	dev->isr_installed = FALSE;
	return dev;
}

upm_result_t upm_ttp223_close(void* dev){
	upm_ttp223 device = (upm_ttp223) dev;
	if(device->gpio != NULL)
		mraa_gpio_close(device->gpio);
	if(device != NULL)
		free(device);
	return UPM_SUCCESS;
}

upm_result_t upm_ttp223_get_value(mraa_gpio_context dev, upm_boolean_t* value){
	*value = mraa_gpio_read(dev);
	return UPM_SUCCESS;
}

upm_result_t upm_ttp223_is_pressed(void* dev, upm_boolean_t* value, int num){
	upm_ttp223 device = (upm_ttp223) dev;
	if(num != 1){
		printf("Sorry this sensor driver supports only one touch pad\n");
		return UPM_ERROR_OUT_OF_RANGE;
	}
	upm_boolean_t ret = FALSE;
	upm_ttp223_get_value(device->gpio, &ret);
	*value = ret;
	return UPM_SUCCESS;
}

upm_result_t upm_ttp223_install_isr(void* dev, mraa_gpio_edge_t edge_level, void (*isr)(void *), void *arg){
	upm_ttp223 device = (upm_ttp223) dev;
	if(device->isr_installed)
		upm_ttp223_uninstall_isr(device);

	mraa_gpio_isr(device->gpio, edge_level, isr, arg);
	device->isr_installed = TRUE;
	return UPM_SUCCESS;
}

upm_result_t upm_ttp223_uninstall_isr(void* dev){
	upm_ttp223 device = (upm_ttp223) dev;
	mraa_gpio_isr_exit(device->gpio);
	device->isr_installed = FALSE;
	return UPM_SUCCESS;
}
