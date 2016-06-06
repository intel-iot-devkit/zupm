// To run:
// build the 2 drivers in the folder as a part of UPM
// and update edison with this built version of UPM
// copy over this file to the edison
// set library path as: LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib
// compile as: gcc main_prog.c -ldl -I/usr/include/upm
// run as: ./a.out

#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include "upm_servo.h"

int main(int argc, char **argv) {
	void *handle;
	struct _upm_servo_ft (*getft)();
	char *error;

	handle = dlopen("/usr/lib/libupm-upm_c_es08a.so", RTLD_LAZY);
	if(!handle){
		printf("Error in fetching handle");
		exit(1);
	}

	getft = dlsym(handle, "upm_servo_get_function_table");
	if((error =dlerror()) != NULL){
		fputs(error, stderr);
		exit(1);
	}

	struct _upm_servo_ft ft = (*getft)();
	void* dev = ft.upm_servo_init(3,3,600,2200);
	printf("starting es08a motor\n");
	ft.upm_servo_set_angle(dev, 180);
	sleep(1);
	ft.upm_servo_set_angle(dev, 90);

	dlclose(handle);
}
