#include <stdio.h>
#include <stdlib.h>
#include "cli.h"
#include "adc.h"
#include "gpio.h"
#include "gpio_alias.h"

command_status do_ar(int argc, char *argv[]) {
	char portTemp;
	uint8_t pin = 0, i;
	GPIO_TypeDef * port;
	GPIO gpio;


	if (argc == 1){
		for (i = 0; i < NUM_ADC; i++){
			if (getGpioAlias((GPIO_TypeDef **)&(ADC_LUT[i].port), (uint8_t *)&(ADC_LUT[i].pin), &gpio))
			printf("%s: %d\r\n", gpio.name, analogRead(ADC_LUT[i].adc, ADC_LUT[i].channel));
			else
			printf("Result: %d\r\n", analogRead(ADC_LUT[i].adc, ADC_LUT[i].channel));
		}
		return SUCCESS;
	}

	portTemp = argv[1][0];

	if (hasGpioAlias(&port, &pin, argv[1])){
		for (i = 0; i < NUM_ADC; i++){
			if (ADC_LUT[i].port == port && ADC_LUT[i].pin == pin){
				printf("Result: %d\r\n", analogRead(ADC_LUT[i].adc, ADC_LUT[i].channel));
				return SUCCESS;
			}
		}
	}

	port = gpio_getGpioPort(portTemp);
	pin = atoi((const char *) &argv[1][1]);

	for (i = 0; i < NUM_ADC; i++){
		if (ADC_LUT[i].port == port && ADC_LUT[i].pin == pin){
			printf("Result: %d\r\n", analogRead(ADC_LUT[i].adc, ADC_LUT[i].channel));
			return SUCCESS;
		}
	}
	return USAGE;
}

COMMAND_ENTRY("ar", "ar <port[pin] | info>", "Perform ADC conversions on available pins.", do_ar)

