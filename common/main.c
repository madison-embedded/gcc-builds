#include <stdio.h>
#include "cli.h"
#include "timer.h"

int main(void) {

	unsigned int curr = 0, prev = 0;

	if (!board_init())
		fault();

	while(1) {
		/* Handle Button */
		if (readButton()){
			setLED(1, true);
			setLED(0, true);
		}
		else {
			setLED(1, false);
			setLED(0, false);
		}

		check_input();

		/* LwIP main loop structure
		if (eth_initialized) {
			if (!(ticks % link_up_check_time)) {
				// ready PHY to check link status
				// if link down, clear link_up in netif
			}
			if (link_up) {
				// disable interrupts
				ethernetif_input(&gnetif);
				// enable interrupts
			}
			sys_check_timeouts();
		}*/

		/* Blink Red LED */
		curr = ticks / 250;
		if (curr != prev)
			(curr % 2) ? 
				setLED(2, true) : setLED(2, false);
		prev = curr;
	}
}

