#include <stdio.h>
#include <stdlib.h>
#include "cli.h"
#include "adc.h"
#include "gpio.h"

/* TODO: map out legal pins, right now this reads 'A0' (A3) */
/* TODO: implement 'info' */

command_status do_ar(int argc, char *argv[]) {
	if (argc < 2) return USAGE;
	char port = argv[1][0];
	uint8_t i;
	uint8_t pin = atoi((const char *) &argv[1][1]);
	
	for (i = 0; i < NUM_ADC; i++){
		if (ADC_LUT[i].port == port && ADC_LUT[i].pin == pin)
			printf("Result: %d\r\n", analogRead(ADC_LUT[i].adc, ADC_LUT[i].channel));
	}


	return SUCCESS;
}
COMMAND_ENTRY("ar", "ar <port[pin] | info>", "Perform ADC conversions on available pins.", do_ar)

