#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "mpu9250.h"
extern I2C_HandleTypeDef hi2c;

command_status do_mpu(int argc, char *argv[]) {
int i;
uint8_t pData;


	if (strcmp(argv[1], "init") == 0)
	MPUinitialize();
else if (strcmp(argv[1], "read") == 0)
	MPUread();
else if (strcmp(argv[1], "step") == 0)
	MPU_step();
else if (strcmp(argv[1], "reg") == 0){
	for (i = 0; i < 127; i++){
HAL_I2C_Mem_Read(&hi2c, MPU_ADDRESS<<1, i, 1, &pData, 1, 500);
		printf("0x%d Data: 0x%x\r\n", i, pData);

	}
}else 
	printOffsets();

	

	return SUCCESS;

}
COMMAND_ENTRY("mpu", "mpu <port>[num] [duty cycle]", "Perform live initialization of pulse with modulation for specified pins.", do_mpu)
