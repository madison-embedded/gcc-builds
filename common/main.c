#include "config.h"
#include "cli.h"

void print_post_info(void) {

}

int main(void) {

	if (!board_init()) fault();

	print_post_info();

	while(1) {
		if (readButton()) setLEDs(true);
		else setLEDs(false);
		processCommand();
	}
}

