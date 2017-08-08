#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hal/hal_conf.h"
#include "hal/stm32f7xx_hal_i2c.h"
#include "cli.h"

extern I2C_HandleTypeDef hi2c;


char * getStatus(HAL_StatusTypeDef status){
	
	switch (status){
		case HAL_OK:
			return "OK";
		case HAL_ERROR:
			return "ERROR";
		case HAL_BUSY:
			return "BUSY";
		case HAL_TIMEOUT:
			return "TIMEOUT";
		default:
			return "NOTGOOD";
	}
	return "NOTGOOD";
}

command_status do_i2c(int argc, char *argv[]) {
	uint8_t address;

	printf("%s\r\n", getStatus(HAL_I2C_Init(&hi2c)));

	for (address = 0; address < 128; address++){

  		printf("%x: %s\r\n", address, getStatus(HAL_I2C_IsDeviceReady(&hi2c, address<<1, 1, 5000)));
	
	}
	return SUCCESS;

}
COMMAND_ENTRY("i2c", "i2c <read | write> <address> <offset>", "Perform live interaction with the i2c devices default list all devices", do_i2c)

