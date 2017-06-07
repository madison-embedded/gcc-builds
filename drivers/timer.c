#include "timer.h"

void delay_ms(unsigned int period) {
	unsigned int curr_count = ticks;
	while (ticks < curr_count + period) {;}
}
