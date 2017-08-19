#include <stdio.h>
#include "cli.h"
#include "timer.h"
#include "ethernetif.h"

inline void blink_handler(unsigned int curr, unsigned int prev) {
	curr = ticks / 250;
	if (curr != prev)
		(curr % 2) ?
			setLED(2, true) : setLED(2, false);
	prev = curr;
}

void __attribute__((weak)) application_handler(void) {
	/* does nothing */
}

int main(void) {

	unsigned int curr = 0, prev = 0;

	if (!board_init())
		fault();

	while(1) {

		/* USB UART */
		check_input();

		/* check network interface */
		lwip_loop_handler();

		/* Badgerloop: DAQ, telemetry, state machine */
/*		application_handler();*/

		/* Blink Red LED */
		blink_handler(curr, prev);
	}
}

void button_handler(void) {
	if (readButton()) {
		setLED(1, true);
		setLED(0, true);
	}
	else {
		setLED(1, false);
		setLED(0, false);
	}
}

