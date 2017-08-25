#ifndef _exti__H__
#define _exti__H__

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "gpio.h"

#define NUMBER_INTERUPT_PINS 16
#define AVERAGE_SIZE		 3


typedef struct{
	int prev;
	int curr;
	int count;
	uint32_t filter[AVERAGE_SIZE];
}timeStamp;

timeStamp interLine[NUMBER_INTERUPT_PINS];

void exti_config(GPIO_TypeDef * port, uint32_t pin, bool rtsr, bool ftsr, bool ie);
timeStamp * getTimeStamps(int pin);

#endif
