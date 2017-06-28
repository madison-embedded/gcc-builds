#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "gpio.h"

void gpio_printPinInfo(GPIO_TypeDef* port, uint8_t pin){

	printf("%c%2d\t", getGpioPortChar(port), pin);
	
	switch (gpio_getMode(port, pin)) {
		case OUTPUT:	printf("out %x", gpio_readPin(port, pin)); break;
		case INPUT:		printf("in  %x", gpio_readPin(port, pin)); break;
		case ALT:		printf("alt"); break;
		case ANALOG:	printf("ana"); break;
		default:		printf("UNKNOWN");
	}

	printf("\tSpeed: ");
	switch (gpio_getSpeed(port, pin)) {
		case LOW_SPEED:			printf("low"); break;
		case MEDIUM_SPEED:		printf("medium"); break;
		case HIGH_SPEED:		printf("high"); break;
		case VERY_HIGH_SPEED:	printf("vhigh"); break;
		default:				printf("UNKNOWN");
	}

	printf("\tPull: ");
	switch (gpio_getPullupState(port, pin)){
		case NONE:		printf("none"); break;
		case PULL_UP:	printf("up"); break;
		case PULL_DOWN:	printf("down"); break;
		default:		printf("UNKNOWN");
	}
	printf("\r\n");
}

command_status do_pin(int argc, char *argv[]) {
	if (argc < 2) return USAGE;
	bool pinOnly = false;
	char portTemp = argv[1][0];
	int pin = 0, i;
	GPIO_TypeDef * port = getGpioPort(portTemp);
	GPIO_MODE mode;
	
	/* check valid GPIO address */
	if (!IS_GPIO_ALL_INSTANCE(port) !=0 ) return USAGE;

	/* check valid pin number */
	if (strlen(argv[1]) > 1) {
		pin = atoi((const char *) &argv[1][1]);
		pinOnly = true;
		if (pin > 15) return USAGE;
	}

	/* print pin information */
	if (argc == 2 || strcmp(argv[2], "info") == 0) {
		if (pinOnly) gpio_printPinInfo(port, pin);
		else for (i=0; i<16; i++) gpio_printPinInfo(port, i);
	}

	/* attempt to set pin */
	else if (strcmp(argv[2], "on") == 0) {
		mode = gpio_getMode(port, pin);
		if (mode == ALT || mode == ANALOG) {
			printf("Pin is either alternate function or analog \r\n");
			return FAIL;
		}
		gpio_setMode(port, pin, OUTPUT);
		gpio_writePin(port, pin, 1);
	}

	/* attempt to reset pin */
	else if (strcmp(argv[2], "off") == 0) {
		mode = gpio_getMode(port, pin);
		if (mode == ALT || mode == ANALOG) {
			printf("Pin is either alternate function or analog \r\n");
			return FAIL;
		}
		gpio_setMode(port, pin, OUTPUT);
		gpio_writePin(port, pin, 0);
	}
	else return USAGE;

	return SUCCESS;
}
COMMAND_ENTRY("pin", "pin <port>[num] [info | on | off]", "Perform live manipulation of GPIO pins.", do_pin)

