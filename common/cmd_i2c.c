#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hal/hal_conf.h"
#include "config.h"
#include "hal/stm32f7xx_hal_i2c.h"
#include "cli.h"
#include "honeywell.h"
#include "mpu9250.h"

extern I2C_HandleTypeDef hi2c;

const char *HALstatusString(HAL_StatusTypeDef status) {
	switch (status) {
		case HAL_OK:		return "OK";
		case HAL_ERROR:		return "ERROR";
		case HAL_BUSY:		return "BUSY";
		case HAL_TIMEOUT:	return "TIMEOUT";
	}
	return "NOTGOOD";
}

command_status do_i2c(int argc, char *argv[]) {
	uint16_t address, memAddr;
	uint8_t numBytes, *dataBuffer = NULL;
	int i;
#if !NETWORKING
	uint32_t mpu_ts;
#endif
	HAL_StatusTypeDef stat;

	/* one argument polls attached devices */
	if (argc == 1) {
		for (address = 0; address < 128; address++)
			if (HAL_I2C_IsDeviceReady(&hi2c, address << 1, 1, 500) == HAL_OK)
				printf("0x%x: found\r\n", address);
		return SUCCESS;
	}

	if (!strcmp("honeywell", argv[1])) {
		if (honeywell_isAlive())
			printf("Pressure:\t%d\r\nTemperature:\t%d\r\n", honeywell_readPressure(), honeywell_readTemperature());
		else printf("Honeywell sensor not present at address 0x%x\r\n", HONEYWELL_I2C_ADDR);
		return SUCCESS;
	}

	if (!strcmp("mpu", argv[1])) {
		int16_t data[3];

		if (argc != 3) return USAGE;
		switch (argv[2][0]){
		case 'i':
			printf("%s\r\n", initMPU9250() ? "OK" : "FAIL");
			break;
		case 'r':
#if NETWORKING
			printf("X\tY\tZ\r\n");
			readAccelData(data);
			printf("%-6d\t%-6d\t%-6d\r\n", to_cms2(data[0]), to_cms2(data[1]), to_cms2(data[2]));
#else
			mpu_ts = ticks;
			printf("X\tY\tZ\r\n");
			do {
				readAccelData(data);
				__disable_irq();
				printf("%-6d\t%-6d\t%-6d\r\n", to_cms2(data[0]), to_cms2(data[1]), to_cms2(data[2]));
				__enable_irq();
				fflush(stdout);
			} while(ticks - mpu_ts < 5000);
			printf("\n");
#endif
			break;
		case 'p': printMPU9250(); break;
		case 'c': calibrate_mpu9250(); break;
		default: return USAGE;
		}
		return SUCCESS;
	}
	
	address = strtoul(argv[2], NULL, 16);
	if (address > 127) {
		printf("Address out of range: 0x%x\r\n", address);
		return USAGE;
	}

	stat = HAL_I2C_IsDeviceReady(&hi2c, address << 1, 1, 500);
	if (stat != HAL_OK) {
		printf("0x%x: %s\r\n", address, HALstatusString(stat));
		return USAGE;
	}

	if (!strcmp(argv[1], "write")) {

		memAddr = atoi((const char *) argv[3]);

		if (argc != 5) return USAGE;

		/* populate array of outgoing data */
		dataBuffer = calloc(argc - 4, 1);
		if (dataBuffer == NULL)
			return FAIL;
		for (i = 4; i < argc; i++)
			dataBuffer[i - 4] = (uint8_t) strtoul(argv[i], NULL, 16);

		printf("%s\r\n", HALstatusString(HAL_I2C_Mem_Write(&hi2c, address << 1, memAddr, (strlen(argv[2]) > 4) ? 2 : 1, dataBuffer, argc - 4, 500)));

		free(dataBuffer);
		return SUCCESS;

	} else if (!strcmp(argv[1], "read")) {

		/* not reading from a memory address */
		if (argc == 4) {

			numBytes = atoi((const char *) argv[3]);
			dataBuffer = calloc(numBytes, 1);

			stat = HAL_I2C_Master_Receive(&hi2c, address << 1, dataBuffer, numBytes, 500);
			if (stat != HAL_OK) {
				printf("%s\r\n", HALstatusString(stat));
				return FAIL;
			}

			for (i = 0; i < numBytes; i ++)
				printf("0x%x\r\n", (dataBuffer[i]));

			free(dataBuffer);
			return SUCCESS;	
		}

		memAddr = atoi((const char *) argv[3]);
		numBytes = atoi((const char *) argv[4]);
		dataBuffer = calloc(numBytes, 1);

		stat = HAL_I2C_Mem_Read(&hi2c, address << 1, memAddr, (strlen(argv[2]) > 4) ? 2 : 1, dataBuffer, numBytes, 500);
		if (stat != HAL_OK) {
			printf("%s\r\n", HALstatusString(stat));
			return FAIL;
		}

		for (i = 0; i < numBytes; i ++)
			printf("0x%x\r\n", (dataBuffer[i]));

		free(dataBuffer);
		return SUCCESS;
	}

	return USAGE;
}
COMMAND_ENTRY("i2c", "i2c <read | write> <address HEX> <offset | num bytes> [value HEX]", "Perform live interaction with the i2c devices default list all devices", do_i2c)

