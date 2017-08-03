#include <stdio.h>
#include "cli.h"
#include "timer.h"

/*****************************************************************************/
#include "ethernetif.h"
#include "lwip/timeouts.h"
extern int eth_inited;
#define LINK_UP_CHECK_TIME	5000
/*****************************************************************************/

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

		/*********************************************************************/
		if (eth_inited) {
			if (!(ticks % LINK_UP_CHECK_TIME)) {
				// ready PHY to check link status
				// if link down, clear link_up in netif
			}
			if (gnetif.flags & NETIF_FLAG_LINK_UP) {
				__disable_irq();
				ethernetif_input(&gnetif);
				__enable_irq();
			}
			sys_check_timeouts();
		}
		/*********************************************************************/

		/* Blink Red LED */
		curr = ticks / 250;
		if (curr != prev)
			(curr % 2) ? 
				setLED(2, true) : setLED(2, false);
		prev = curr;
	}
}

