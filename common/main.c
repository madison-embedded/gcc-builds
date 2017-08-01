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

		/* Blink Red LED */
		curr = ticks / 250;
		if (curr != prev)
			(curr % 2) ? 
				setLED(2, true) : setLED(2, false);
		prev = curr;
	}
}

