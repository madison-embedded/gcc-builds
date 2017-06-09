#include "config.h"
#include "cli.h"

void print_post_info(void) {

}

int main(void) {

	unsigned int curr = 0, prev = 0;
	void *malloc_test;

	if (!board_init())
		fault();

	print_post_info();

	malloc_test = malloc(4);

	/* TODO: get this to not happen */
	/* at least it works at all! */
	if (!malloc_test) {
		while(1) {
			curr = ticks / 1000;
			if (curr != prev)
				(curr % 2) ? 
					setGreen(true) : setGreen(false);
			prev = curr;
		}
	}

	while(1) {

		/* Handle Button */
		if (readButton()) {
			setBlue(true);
			setGreen(true);
		}
		else {
			setBlue(false);
			setGreen(false);
		}

		//processCommand();

		/* Blink Red LED */
		curr = ticks / 1000;
		if (curr != prev)
			(curr % 2) ? 
				setRed(true) : setRed(false);
		prev = curr;
	}
}

