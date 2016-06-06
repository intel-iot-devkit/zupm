//Modified: Abhishek Malik <abhishek.malik@intel.com>
// no compile time check of variable args - if you get them wrong its a seg fault

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "upm_servo.h"
#include "upm.h"

int main() {
	///////////// Calling read/write directly ///////////
	void* dev = upm_sensor_init(3, 3, 600, 2200);
	printf("first print\n");
	servo_data_struct str = {180};
	upm_sensor_write(dev, &str, 0);
	sleep(1);
	str.angle = 90;
	upm_sensor_write(dev, &str, 0);

	///////////// Calling function table function ///////////
	/*void* ft = upm_get_function_table(UPM_SERVO);
	struct _upm_servo_ft sft = *(struct _upm_servo_ft*)ft;
	void* dev = upm_sensor_init(3, 3, 600, 2200);
	printf("first print\n");
	sft.upm_servo_set_angle(dev, 180);
	sleep(1);
	sft.upm_servo_set_angle(dev, 90);*/

	/*void* ft = upm_get_function_table(UPM_TOUCH);
	struct _upm_touch_ft tft = *(struct _upm_touch_ft*) ft;
	void* dev = tft.upm_touch_init(1, 2);
	printf("first print\n");
	upm_boolean_t val;
	while(1){
		tft.upm_touch_get_is_pressed(dev, &val, 1);
		printf("is the sensor pressed: %d\n", val);
		upm_delay_milliseconds(500);
	}
	tft.upm_touch_close(dev);*/


	/*void* ft = upm_get_function_table(UPM_GENERIC);
	struct _upm_generic_ft gft = *(struct _upm_generic_ft*) ft;
	void* dev = gft.upm_sensor_init(1,0);
	//ft = upm_get_function_table(UPM_MOISTURE);
	//struct _upm_moisture_ft mft = *(struct _upm_moisture_ft*) ft;
	printf("first print\n");
	int val;
	while(1){
		gft.upm_sensor_read(dev, &val);
		printf("value: %d\n", val);
		upm_delay_seconds(1);
	}*/
	return 0;
}

