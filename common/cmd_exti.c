#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "exti.h"
#include "gpio_alias.h"
#include "retro.h"

command_status do_exti(int argc, char *argv[]) {
	retroInit();
	if (argc < 2) return USAGE;
	uint8_t pin = 0;
	GPIO_TypeDef * port; 

	if(hasGpioAlias(&port, &pin, argv[1])){
		getTimeStamps(pin);
		printf("Velocity %d cm/s\r\n", getVelocity());
		return SUCCESS;
	}

	pin = atoi((const char *) argv[1]);
 	
	getTimeStamps(pin);
	printf("Velocity %d cm/s\r\n", getVelocity());

	return SUCCESS;
}
COMMAND_ENTRY("exti", "exti <pin>", "Prints current and previous time stamp of the External Interrpt. Only on interrupt per pin number", do_exti)

