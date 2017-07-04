#include <stdio.h>
#include "cli.h"
#include "adc.h"
#include "gpio.h"

/* TODO: map out legal pins, right now this reads 'A0' (A3) */
/* TODO: implement 'info' */
command_status do_ar(int argc, char *argv[]) {
	uint16_t result = 0;
	gpio_setClock(GPIOA, true);
	gpio_setMode(GPIOA, 3, ANALOG);
	result = analogRead(ADC1, GPIOA, 3);
	printf("Result: %d\r\n", result);
	return SUCCESS;
}
COMMAND_ENTRY("ar", "ar <pin | info>", "Perform ADC conversions on available pins.", do_ar)

