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
	uint16_t address;
	uint8_t pData;
	uint8_t* pBuffer = NULL;
	uint16_t MemAdd;
	uint8_t numBytes;
	int i;

	/* one argument checks what devices are attached */
	if (argc == 1) {
		for (address = 0; address < 128; address++){
			printf("0x%x: %s\r\n", address, getStatus(HAL_I2C_IsDeviceReady(&hi2c, address<<1, 1, 5000)));
		}
		return SUCCESS;
	}
	
	/*  */
	if (argc < 3 || argc > 6)
		return USAGE;

	address = strtoul(argv[2], NULL, 16);

	MemAdd = atoi((const char *) argv[3]);
	
	if (strcmp(argv[1], "write") == 0) {
	
		if (argc != 5)
			return USAGE;
	 	pData= strtoul(argv[4], NULL, 16);
	 	
	 	if (HAL_I2C_IsDeviceReady(&hi2c, address<<1, 1, 5000) != HAL_OK){
	 		printf("0x%x: %s\r\n", address, getStatus(HAL_I2C_IsDeviceReady(&hi2c, address<<1, 1, 500)));
	 		return USAGE;
	 	}
	 	printf("%s\r\n",getStatus(HAL_I2C_Mem_Write(&hi2c, address<<1, MemAdd, 1, &pData, 1, 500)));
	 	return SUCCESS;
	}
	if (strcmp(argv[1], "read") == 0) {
		if (argc < 5){
			numBytes = atoi((const char *) argv[3]);
			
			printf("%s\r\n", getStatus(HAL_I2C_Master_Receive(&hi2c, address<<1, pBuffer , numBytes, 500)));
			
			for (i = 0; i < numBytes; i ++)
				printf("0x%x\r\n", (pBuffer[i]));
		
		
		return SUCCESS;	

		}
		else{
	
		if (HAL_I2C_IsDeviceReady(&hi2c, address<<1, 1, 5000) != HAL_OK){
			printf("0x%x: %s\r\n", address, getStatus(HAL_I2C_IsDeviceReady(&hi2c, address<<1, 1, 500)));
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

