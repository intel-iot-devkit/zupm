/*
 * upm_utilities.h
 *
 *  Created on: May 18, 2016
 *      Author: malikabh
 */

#ifndef UPM_UTILITIES_H_
#define UPM_UTILITIES_H_

typedef enum {
	UPM_SUCCESS,						/* Operation is successful */
	UPM_MODE_NOT_SUPPORTED,				/* Trying to access one mode while working on another mode */
	UPM_ERROR_UNSPECIFIED,				/* Unspecified error */
	UPM_ERROR_OUT_OF_RANGE,				/* When the input to drive is too high/low or -ve */
	UPM_ERROR_OPERATION_FAILED,			/* When a function isn't able to perform as expected */
	UPM_ERROR_TIMED_OUT,
	UPM_ERROR_INVALID_PACKET_SIZE
} upm_result_t;

typedef enum {
	UPM_ANALOG_MODE,
	UPM_GPIO_MODE,
	UPM_PWM_MODE,
	UPM_I2C_MODE,
	UPM_SPI_MODE,
	UPM_UART_MODE
} upm_protocol_modes;

typedef enum {
	UPM_DISTANCE,
	UPM_TEMPERATURE,
	UPM_PRESSURE,
	UPM_SERVO,
	UPM_LIGHT,
	UPM_STEPPER,
	UPM_HEART_RATE
} upm_sensor_types;

void* upm_get_function_table(upm_sensor_types sensor_type);

static inline void upm_delay_seconds(int time){
#ifdef linux
	sleep(time);
#endif
}

static inline void upm_delay_milliseconds(int time){
#ifdef linux
	usleep(1000*time);
#endif
}

static inline void upm_delay_microseconds(int time){
#ifdef linux
	usleep(time);
#endif
}

#endif /* UPM_UTILITIES_H_ */
