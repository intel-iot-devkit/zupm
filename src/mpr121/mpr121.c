/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * 		   Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "mpr121.h"

struct _upm_mpr121 {
	mraa_i2c_context	i2c;
	int					bus;
	uint8_t 			address;
	uint8_t				over_current_fault;
};

const char upm_mpr121_name[] = "MPR121";
const char upm_mpr121_description[] = "MPR121 Touch Sensor";
const upm_protocol_t upm_mpr121_protocol[] = {UPM_I2C};
const upm_sensor_t upm_mpr121_category[] = {UPM_SWITCH};


upm_sensor_descriptor_t upm_mpr121_get_descriptor (void* dev){
	upm_sensor_descriptor_t usd;
	usd.name = upm_mpr121_name;
	usd.description = upm_mpr121_description;
	usd.protocol_size = 1;
	usd.protocol = upm_mpr121_protocol;
	usd.category_size = 1;
	usd.category = upm_mpr121_category;
	return usd;
}

void* upm_mpr121_get_ft(upm_sensor_t sensor_type){
	if(sensor_type == UPM_SWITCH){
		struct _upm_switch_ft *sft = malloc(sizeof(*sft));
		if(sft == NULL){
			printf("Unable to assign memory to the light structure");
			return NULL;
		}
		sft->upm_switch_get_value = upm_mpr121_read_buttons;
		return sft;
	}
	else if(sensor_type == UPM_SENSOR){
		struct _upm_sensor_ft *ft = malloc(sizeof(*ft));
		if(ft == NULL){
			printf("Unable to assign memory to the light structure");
			return NULL;
		}
		ft->upm_sensor_init_name = upm_mpr121_init_name;
		ft->upm_sensor_close = upm_mpr121_close;
		ft->upm_sensor_read = upm_mpr121_read;
		ft->upm_sensor_write = upm_mpr121_write;
		return ft;
	}
	return NULL;
}

void* upm_mpr121_init_name(int num,...){
	//upm_mpr121 dev = (upm_mpr121) malloc(sizeof(struct _upm_mpr121));
	upm_mpr121 dev  = (upm_mpr121) upm_malloc(sizeof(struct _upm_mpr121));
	va_list pin_list;
	va_start(pin_list, num);
	dev->bus = va_arg(pin_list, int);
	dev->address = va_arg(pin_list, int);
	va_end(pin_list);

	dev->i2c = mraa_i2c_init(dev->bus);
	if(dev->i2c == NULL){
		return NULL;
	}

	if(mraa_i2c_address(dev->i2c, dev->address) != MRAA_SUCCESS){
		printf("error using i2c address: %x\n", dev->address);
		return NULL;
	}

	dev->over_current_fault = false;
	return dev;
}

void* upm_mpr121_init(int bus, uint8_t address){
	upm_mpr121 dev = (upm_mpr121) malloc(sizeof(struct _upm_mpr121));

	dev->bus = bus;
	dev->address = address;
	dev->i2c = mraa_i2c_init(dev->bus);
	if(dev->i2c == NULL){
		return NULL;
	}

	if(mraa_i2c_address(dev->i2c, dev->address) != MRAA_SUCCESS){
		printf("error using i2c address: %x\n", dev->address);
		return NULL;
	}

	dev->over_current_fault = false;
	return dev;
}

void upm_mpr121_close(void* dev){
	upm_mpr121 device = (upm_mpr121) dev;
	mraa_i2c_stop(device->i2c);
	upm_free(device);
}

upm_result_t upm_mpr121_write_bytes(void* dev, uint8_t reg, uint8_t *buffer, int len){
	upm_mpr121 device = (upm_mpr121) dev;
	if(!len || !buffer)
		return UPM_SUCCESS;

	// create a buffer 1 byte larger than the supplied buffer,
	// store the register in the first byte
	uint8_t buf2[len + 1];
	buf2[0] = reg;

	// copy in the buffer after the reg byte

	int i;
	for(i=1; i<(len + 1); i++)
		buf2[i] = buffer[i-1];

	if(mraa_i2c_address(device->i2c, device->address) != MRAA_SUCCESS){
		printf("write byte error address\n");
		return UPM_ERROR_OPERATION_FAILED;
	}

	if(mraa_i2c_write(device->i2c, buf2, len+1) != MRAA_SUCCESS){
		printf("write byte error while writing\n");
		return UPM_ERROR_OPERATION_FAILED;
	}
	return UPM_SUCCESS;
}

upm_result_t upm_mpr121_read_bytes(void* dev, uint8_t reg, uint8_t *buffer, int len){
	upm_mpr121 device = (upm_mpr121) dev;
	if(!len || !buffer)
		return UPM_ERROR_INVALID_SIZE;

	// The usual m_i2c.read() does not work here, so we need to
	// read each byte individually.
	int i;
	for (i=0; i<len; i++)
		buffer[i] = mraa_i2c_read_byte_data(device->i2c, reg+i);

	return UPM_SUCCESS;
}

upm_result_t upm_mpr121_write(void* dev, void* value, int len){
	return UPM_ERROR_NOT_IMPLEMENTED;
}

upm_result_t upm_mpr121_read(void* dev, void* value, int* len){
	upm_mpr121 device = (upm_mpr121) dev;
	uint16_t* states = value;
	uint8_t buffer[2];

	// read in the 2 bytes at register 0x00-0x01, and setup the member
	// variables accordingly.
	upm_mpr121_read_bytes(device, 0x00, buffer, 2);
	*states = (buffer[0] | ((buffer[1] & 0x1f) << 8));
	if (buffer[1] & 0x80)
		device->over_current_fault = true;
	else
		device->over_current_fault = false;
	return UPM_SUCCESS;
}

upm_result_t upm_mpr121_read_buttons(void* dev, uint32_t* states, int num){
	upm_mpr121 device = (upm_mpr121) dev;
	int len;
	upm_mpr121_read(device, states, &len);
	/*
	uint8_t rv;
	uint8_t buffer[2];

	// read in the 2 bytes at register 0x00-0x01, and setup the member
	// variables accordingly.
	upm_mpr121_read_bytes(device, 0x00, buffer, 2);
	 *states = (buffer[0] | ((buffer[1] & 0x1f) << 8));
	if (buffer[1] & 0x80)
		device->over_current_fault = true;
	else
		device->over_current_fault = false;*/
	return UPM_SUCCESS;
}

upm_result_t upm_mpr121_config_an3944(void* dev){
	upm_mpr121 device = (upm_mpr121) dev;
	// Configure the mpr121 chip as recommended in the AN3944 MPR121
	// Quick Start Guide
	// First, turn off all electrodes by zeroing out the Electrode Configuration
	// register.
	// If this one fails, it's unlikely any of the others will succeed.
	printf("Entering configuration function\n");
	uint8_t eleConf = 0x00;
	if(upm_mpr121_write_bytes(device, 0x5e, &eleConf, 1) != UPM_SUCCESS){
		printf("write to electrode configuration register failed\n");
		return UPM_ERROR_OPERATION_FAILED;
	}

	// Section A
	// Filtering when data is greater than baseline
	// regs 0x2b-0x2e
	printf("configuring section A\n");
	uint8_t sectA[] = {0x01, 0x01, 0x00, 0x00};
	if(upm_mpr121_write_bytes(device, 0x2b, sectA, 4) != UPM_SUCCESS){
		printf("write to section a failed\n");
		return UPM_ERROR_OPERATION_FAILED;
	}

	// Section B
	// Filtering when data is less than baseline
	// regs 0x2f-0x32
	printf("configuring section B\n");
	uint8_t sectB[] = {0x01, 0x01, 0xff, 0x02};
	if(upm_mpr121_write_bytes(device, 0x2f, sectB, 4) != UPM_SUCCESS){
		printf("write to section b failed\n");
		return UPM_ERROR_OPERATION_FAILED;
	}

	// Section C
	// Touch Threshold/Release registers, ELE0-ELE11
	// regs 0x41-0x58
	//                	__T_  __R_
	uint8_t sectC[] =  {0x0f, 0x0a,
			0x0f, 0x0a,
			0x0f, 0x0a,
			0x0f, 0x0a,
			0x0f, 0x0a,
			0x0f, 0x0a,
			0x0f, 0x0a,
			0x0f, 0x0a,
			0x0f, 0x0a,
			0x0f, 0x0a,
			0x0f, 0x0a,
			0x0f, 0x0a};

	printf("configuring touch threshold/release regs\n");
	if(upm_mpr121_write_bytes(device, 0x41, sectC, 24) != UPM_SUCCESS){
		printf("failed to configure touch threshold/release regs\n");
		return UPM_ERROR_OPERATION_FAILED;
	}

	// Section D
	// Filter configuration
	// reg 0x5d
	printf("configuring filter stuff\n");
	uint8_t filterConf = 0x04;
	if(upm_mpr121_write_bytes(device, 0x5d, &filterConf, 1) != UPM_SUCCESS){
		printf("unable to configure filters\n");
		return UPM_ERROR_OPERATION_FAILED;
	}

	// Section F
	// Autoconfiguration registers
	// regs 0x7b-0x7f
	printf("configuring auto config regs\n");
	uint8_t sectF0 = 0x0b;
	if(upm_mpr121_write_bytes(device, 0x7b, &sectF0, 1) != UPM_SUCCESS){
		printf("unable to configure auto config regs\n");
		return UPM_ERROR_OPERATION_FAILED;
	}

	uint8_t sectF1[] = {0x9c, 0x65, 0x8c};
	if(upm_mpr121_write_bytes(device, 0x7d, sectF1, 3) != UPM_SUCCESS){
		return UPM_ERROR_OPERATION_FAILED;
	}

	// Section E - this one must be set last, and switches to run mode
	// Enable all 12 electrodes, and set a pre-calibration to avoid
	// excessive calibration delay on startup.
	// reg 0x5e
	eleConf = 0x8c;
	if(upm_mpr121_write_bytes(device, 0x5e, &eleConf, 3) != UPM_SUCCESS){
		return UPM_ERROR_OPERATION_FAILED;
	}
	return UPM_SUCCESS;
}
