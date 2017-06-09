#include "config.h"
#include "cli.h"

void print_post_info(void) {

}

int main(void) {

	unsigned int curr = 0, prev = 0;

	if (!board_init()) fault();

	print_post_info();

	while(1) {

		if (readButton()) {
			setBlue(true);
			setGreen(true);
		}
		else {
			setBlue(false);
			setGreen(false);
		}

		//processCommand();

		curr = ticks / 1000;

		if (curr != prev) {
			(curr % 2) ? setRed(true) : setRed(false);
		}

		prev = curr;
	}
}

