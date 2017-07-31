#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "tim.h"
#include "gpio_alias.h"

command_status do_pwm(int argc, char *argv[]) {
	if (argc < 2) return USAGE;
	bool pinOnly = false, useAlias = false;
	char portTemp = argv[1][0];
	int group;
	uint8_t pin = 0, i;
	GPIO_TypeDef * port; 
	GPIO_MODE mode;

	port = gpio_getGpioPort(portTemp);

	return SUCCESS;
}
COMMAND_ENTRY("pwm", "pwm <port>[num] [duty cycle]", "Perform live initialization of pulse with modulation for specified pins.", do_pwm)

