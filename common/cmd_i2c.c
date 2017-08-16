#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hal/hal_conf.h"
#include "hal/stm32f7xx_hal_i2c.h"
#include "cli.h"
#define OUTPUTMAX			14746
#define OUTPUTMIN			1638
#define PRESSUREMAX			23.206
#define PRESSUREMIN			0
double temp = 0.0;
double pressure, presstemp = 0.0;
uint16_t tempReading, pressureReading;

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
	uint16_t address;
	uint8_t pData;
	uint8_t* pBuffer = NULL;
	uint16_t MemAdd;
	uint8_t numBytes;


	if (argc == 1) {
		for (address = 0; address < 128; address++){
			printf("%x: %s\r\n", address, getStatus(HAL_I2C_IsDeviceReady(&hi2c, address<<1, 1, 5000)));
		}
		return SUCCESS;
	}
	
	if (argc == 2 || argc > 6)
		return USAGE;

	address = strtoul(argv[2], NULL, 16);

	MemAdd = atoi((const char *) argv[3]);
	
	
	

	if (strcmp(argv[1], "write") == 0) {
	
		if (argc != 5)
			return USAGE;
	 	pData= strtoul(argv[4], NULL, 16);
	 	
	 	if (HAL_I2C_IsDeviceReady(&hi2c, address<<1, 1, 5000) != HAL_OK){
	 		printf("%x: %s\r\n", address, getStatus(HAL_I2C_IsDeviceReady(&hi2c, address<<1, 1, 500)));
	 		return USAGE;
	 	}
	 	printf("%s\r\n",getStatus(HAL_I2C_Mem_Write(&hi2c, address<<1, MemAdd, 1, &pData, 1, 500)));
	 	return SUCCESS;
	}
	if (strcmp(argv[1], "read") == 0) {
		if (argc < 5){
		numBytes = atoi((const char *) argv[3]);
 		printf("Reading\r\n");
        	getStatus(HAL_I2C_Master_Receive(&hi2c, address<<1, pBuffer , numBytes, 500));
		printf("%x\r\n", (pBuffer[0]));
		printf("%x\r\n", (pBuffer[1]));
		printf("%x\r\n", (pBuffer[2]));
		printf("%x\r\n", (pBuffer[3]));
	
		pressureReading = (((pBuffer[0] & 0x3f) <<8) | pBuffer[1]);
		printf ("pressureReading is %d\r\n", pressureReading);
 		tempReading = ((pBuffer[2] <<8) | pBuffer[3]) >>5;
 		temp = ((tempReading *200)/2047) -50;
 		presstemp = (pressureReading-OUTPUTMIN)*(PRESSUREMAX-PRESSUREMIN);
		pressure  = (presstemp/(OUTPUTMAX-OUTPUTMIN)) + PRESSUREMIN;
	
		printf("Temperature is: %f Degress Celsius\r\n", temp);
		printf("Pressure is: %f psi\r\n", pressure); 
		return SUCCESS;	

		}
		else{
	
		if (HAL_I2C_IsDeviceReady(&hi2c, address<<1, 1, 5000) != HAL_OK){
			printf("%x: %s\r\n", address, getStatus(HAL_I2C_IsDeviceReady(&hi2c, address<<1, 1, 500)));
			return USAGE;
		}

		printf("%s\r\n",getStatus(HAL_I2C_Mem_Read(&hi2c, address<<1, MemAdd, 1, &pData, 1, 500)));
		printf("address: 0x%x Data: 0x%x\r\n", address, pData);
		return SUCCESS;
		}
	}
	return USAGE;
}
COMMAND_ENTRY("i2c", "i2c <read | write> <address HEX> <offset> [value HEX]", "Perform live interaction with the i2c devices default list all devices", do_i2c)

