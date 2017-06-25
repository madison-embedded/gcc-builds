#include <stdio.h>
//#include "config.h"
#include "cli.h"
#include "timer.h"

extern void print_post_info(void);

int main(void) {

	unsigned int curr = 0, prev = 0;

	if (!board_init())
		fault();

	print_post_info();
	printPrompt();

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

		check_input();

		/* Blink Red LED */
		curr = ticks / 1000;
		if (curr != prev) {
			(curr % 2) ? 
				setLED(0, true) : setLED(0, false);
		}
		prev = curr;
	}
}

