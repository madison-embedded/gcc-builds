#include "HWPressure.h"

uint8_t pressureData[4];
int status = -1;
double temp = 0.0;
double pressure,presstemp = 0.0;

uint16_t tempReading, pressureReading;

 void HPread(void){
 	if (I2C_Master_Receive_Data(PRESSURE_ADDRESS, 4, pressureData)) status =-1;
	if (HAL_I2C_Mem_Read(&hi2c, PRESSURE_ADDRESS,)) status = -1;
 	else status  = (pressureData[0] >>6) &3;
 	
 	pressureReading = (((pressureData[0] & 3f) <<8) | pressureData[1]);
 	tempReading = ((pressureData[2] <<8) | pressureData[3]) >>5;
 	temp = ((tempReading *200)/2047) -50;
 	presstemp = (pressureReading-OUTPUTMIN)*(PRESSUREMAX-PRESSUREMIN);
	pressure  = (presstemp/(OUTPUTMAX-OUTPUTMIN)) + PRESSUREMIN;
 }

 double HPGetStatus(void){
 	return status;
 }

 double HPGetPressure(void){
 	if (status == -1) return -1;
 	return pressure;
 }

double HPGetTemp(void){
	if (status == -1) return -1;
	return temp;

}
