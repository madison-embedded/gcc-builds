#include "adxl335.h"
#include "adc.h"

int bias;

int initAccelAdc(void) {
	bias = analogRead(ADC3, 4);
	return 1;
}

int readAccelAdc(void) {
	int retval = analogRead(ADC3, 4);
	retval -= bias;
	retval *= 11;	
	return retval;
}
