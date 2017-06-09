#include "config.h"
#include "cli.h"

void print_post_info(void) {

}

bool test_malloc(void) {
	void *malloc_test;
	malloc_test = malloc(4);
	if (!malloc_test) return false;
	free(malloc_test);
	return true;
}

int main(void) {

	unsigned int curr = 0, prev = 0;

	if (!board_init())
		fault();

	if (!test_malloc())
		fault();

	print_post_info();

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

