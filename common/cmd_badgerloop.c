#include "badgerloop.h"
#include <stdio.h>
#include "config.h"
#include "cli.h"

void get_performance(void (*func)(void)) {

	uint64_t before = 0, after = 0, clocks_elapsed, time_elapsed;

	before = SysTick->VAL;
	func();
	after = SysTick->VAL;

	/* check for timer overflow (timer counts down) */
	if (after > before)
		clocks_elapsed = before * (ticks * (uint64_t) 16000) - after * (ticks * (uint64_t) 16000);
	else clocks_elapsed = before - after;

	time_elapsed = (clocks_elapsed * 6) + (clocks_elapsed / 4);

	/* @160MHz, 1 cycle = 6.25 ns */
	printf("Took %llu clock cycles (%llu ns)\r\n",
		clocks_elapsed, time_elapsed);
}

command_status do_badgerloop(int argc, char *argv[]) {

	int ret = 0;

	if (argc == 1)
		return USAGE;
	switch (argv[1][0]) {
	/* test dashboard functionality */
	case 'd':
		ret = send_telemetry_to_Dashboard();
		break;
	/* test SpaceX telemetry */
	case 's':
		ret = send_telemetry_to_SpaceX();
		break;
	/* perform DAQ / update values */
	case 'r':
		get_performance(badgerloop_update_data);
		break;
	}
	printf("returned %d\r\n", ret);
	return SUCCESS;
}
COMMAND_ENTRY("badgerloop", "badgerloop <init>", "Debug Badgerloop Networking etc.", do_badgerloop)

