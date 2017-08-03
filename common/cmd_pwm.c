#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "tim.h"
#include "gpio_alias.h"

command_status do_pwm(int argc, char *argv[]) {
	if (argc < 2) return USAGE;
	uint8_t channel = atoi((const char *) argv[1]);
	uint8_t duty;

	if (argc > 2)
		duty = atoi((const char *) argv[2]);
	else
		duty = 50;

	TIM_PWM_Init(TIM1, channel, duty);


	return SUCCESS;

}
COMMAND_ENTRY("pwm", "pwm <port>[num] [duty cycle]", "Perform live initialization of pulse with modulation for specified pins.", do_pwm)

