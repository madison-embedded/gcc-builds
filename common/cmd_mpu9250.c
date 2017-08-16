#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "mpu9250.h"

extern I2C_HandleTypeDef hi2c;

command_status do_mpu(int argc, char *argv[]) {
int i;
uint8_t pData;
int16_t accel[3];
int16_t accel2[3];
uint32_t last_ticks = 0;


	if (strcmp(argv[1], "init") == 0)
	MPUinitialize();
else if (strcmp(argv[1], "read") == 0)
	MPUread();
else if (strcmp(argv[1], "accel") == 0){
	readAccelData(accel2);
	while(1){
		if((ticks % 100 == 0) && ticks != last_ticks){
			last_ticks = ticks;
			readAccelData(accel);
			printf("accel x: %04d, y: %04d, z: %04d,\r", accel[0], accel[1], accel[2]);
		}
	}
}else if (strcmp(argv[1], "reg") == 0){
	for (i = 0; i < 127; i++){
HAL_I2C_Mem_Read(&hi2c, MPU_ADDRESS<<1, i, 1, &pData, 1, 500);
		printf("0x%d Data: 0x%x\r\n", i, pData);

	}
}

	

	return SUCCESS;

}
COMMAND_ENTRY("mpu", "mpu <port>[num] [duty cycle]", "Perform live initialization of pulse with modulation for specified pins.", do_mpu)
