#include "HWPressure.h"

uint8_t pressureData[4];
int HWStatus = -1;
double temp = 0.0;
double pressure,presstemp = 0.0;

uint16_t tempReading, pressureReading;

void HPread(void){
	if (HAL_I2C_Master_Receive(&hi2c, PRESSURE_ADDRESS << 1, pressureData, 4, 500)) HWStatus =-1; 
	else HWStatus  = (pressureData[0] >>6) &3;

	pressureReading = (((pressureData[0] & 0x3f) <<8) | pressureData[1]);
	tempReading = ((pressureData[2] <<8) | pressureData[3]) >>5;
	temp = ((tempReading *200)/2047) -50;
	presstemp = (pressureReading-OUTPUTMIN)*(PRESSUREMAX-PRESSUREMIN);
	pressure  = (presstemp/(OUTPUTMAX-OUTPUTMIN)) + PRESSUREMIN;
}

double HPGetHWStatus(void){
	return HWStatus;
}

double HPGetPressure(void){
	if (HWStatus == -1) return -1;
	return pressure;
}

double HPGetTemp(void){
	if (HWStatus == -1) return -1;
	return temp;
}
