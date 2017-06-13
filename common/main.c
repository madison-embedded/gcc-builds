#include <stdio.h>
#include "config.h"
#include "cli.h"
#include "timer.h"

void fault(void) {
	
	setLED(0, true);

	printf("Entered fault!\r\n");
	printf("TODO: add arguments to this function to trace fault.\r\n");
	
	while (1) {
		setLED(1, true);
		delay_ms(1000);
		setLED(1, false);
		delay_ms(1000);
	}
}

void print_post_info(void) {
	printf("\rSystemCoreClock: %u kHz\r\n", (unsigned int) SystemCoreClock / 1000);
	printPrompt();
}

int main(void) {

	unsigned int curr = 0, prev = 0;

	if (!board_init())
		fault();

	print_post_info();

	while(1) {

		/* Handle Button */
		if (readButton()) {
			setLED(1, true);
			setLED(2, true);
		}
		else {
			setLED(1, false);
			setLED(2, false);
		}

		processCommand();

		/* Blink Red LED */
		curr = ticks / 1000;
		if (curr != prev) {
			(curr % 2) ? 
				setLED(0, true) : setLED(0, false);
		}
		prev = curr;
	}
}

