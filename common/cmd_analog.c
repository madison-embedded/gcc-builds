#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "adc.h"
#include "gpio.h"
#include "gpio_alias.h"
#include "config.h"
#include "adxl335.h"

uint16_t get_performanceITI(uint16_t(*func)(ADC_TypeDef *, uint8_t), ADC_TypeDef * adc, uint8_t pin);

extern int isGroup(char * group);

command_status do_ar(int argc, char *argv[]) {
	char portTemp;
	uint8_t pin = 0, i, ii;
	GPIO_TypeDef * port;
	GPIO gpio;
	int group;


	if (argc == 1){
		for (i = 0; i < NUM_ADC; i++){
			if (getGpioAlias((GPIO_TypeDef **)&(ADC_LUT[i].port), (uint8_t *)&(ADC_LUT[i].pin), &gpio))
				printf("%s: %d\r\n", gpio.name, analogRead(ADC_LUT[i].adc, ADC_LUT[i].channel));
			else
				printf("Result: %d\r\n", analogRead(ADC_LUT[i].adc, ADC_LUT[i].channel));
		}
		return SUCCESS;
	}

	if(strcmp("accel", argv[1]) == 0) {
		uint32_t ts = ticks;
		printf("Accel: \r\n");
		do {
			printf("%5d\r", readAccelAdc());
			fflush(stdout);
		} while (ticks - ts < 5000);

		printf("\n");
		return SUCCESS;
	}

	portTemp = argv[1][0];

	group = isGroup(argv[1]);
	if (group != -1) {
		for (i=0; i<NUM_GPIO_ALIAS; i++) {
			if (group & GPIO_TABLE[i].group) {
				for (ii = 0; ii < NUM_ADC; ii++){
					if (ADC_LUT[ii].port == GPIO_TABLE[i].port && ADC_LUT[ii].pin == GPIO_TABLE[i].pin){
						printf("%s: %d\r\n", GPIO_TABLE[i].name, analogRead(ADC_LUT[ii].adc, ADC_LUT[ii].channel));
					}
				}
			}
		}
		return SUCCESS;
	}

	if (hasGpioAlias(&port, &pin, argv[1])){
		for (i = 0; i < NUM_ADC; i++){
			if (ADC_LUT[i].port == port && ADC_LUT[i].pin == pin){
				if (argc>2) {
					if (strcmp(argv[2], "perf") == 0)
						get_performanceITI(analogRead, ADC_LUT[i].adc, ADC_LUT[i].channel);
				}
				printf("Result: %d\r\n", analogRead(ADC_LUT[i].adc, ADC_LUT[i].channel));

				return SUCCESS;
			}
		}
	}

	port = gpio_getGpioPort(portTemp);
	pin = atoi((const char *) &argv[1][1]);

	for (i = 0; i < NUM_ADC; i++){
		if (ADC_LUT[i].port == port && ADC_LUT[i].pin == pin){
			if (strcmp(argv[1], "perf") == 0)
				get_performanceITI(analogRead, ADC_LUT[i].adc, ADC_LUT[i].channel);
			printf("Result: %d\r\n", analogRead(ADC_LUT[i].adc, ADC_LUT[i].channel));
			return SUCCESS;
		}
	}
	return USAGE;
}

COMMAND_ENTRY("ar", "ar <port[pin] | info>", "Perform ADC conversions on available pins.", do_ar)

