#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "exti.h"
#include "gpio_alias.h"
#include "retro.h"

void printStamp (int pin, timeStamp * stamp) {
	int i;
	printf("addr %p\r\n", stamp);
	printf("%-10s:%d\r\n", "Pin", pin);
	printf("%-10s:%x\r\n", "Previous", stamp->prev);
	printf("%-10s:%x\r\n", "Current", stamp->curr);
	printf("%-10s:%x\r\n", "Count", stamp->count);
	for(i = 0; i < AVERAGE_SIZE; i++)
	printf("Filter[i]:%lu\r\n", stamp->filter[i]);

	printf("main\r\n");
	printf("addr %p\r\n", mainRetro);
	printf("%-10s:%x\r\n", "Previous", mainRetro->prev);
	printf("%-10s:%x\r\n", "Current", mainRetro->curr);
	printf("%-10s:%x\r\n", "Count", mainRetro->count);
	for(i = 0; i < AVERAGE_SIZE; i++)
	printf("Filter[i]:%lu\r\n", mainRetro->filter[i]);
}

command_status do_exti(int argc, char *argv[]) {
	initRetro();
	if (argc < 2) return USAGE;
	uint8_t pin = 0;
	GPIO_TypeDef * port; 

	if(hasGpioAlias(&port, &pin, argv[1])){
		printStamp(pin, getTimeStamps(pin));
		printf("Velocity %d cm/s\r\n", getVelocity());
		return SUCCESS;
	}

	pin = atoi((const char *) argv[1]);
 	
	printStamp(pin, getTimeStamps(pin));
	printf("Velocity %d cm/s\r\n", getVelocity());

	return SUCCESS;
}
COMMAND_ENTRY("exti", "exti <pin>", "Prints current and previous time stamp of the External Interrpt. Only on interrupt per pin number", do_exti)
