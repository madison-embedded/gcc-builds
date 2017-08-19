#include "badgerloop.h"
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "cli.h"
#include "state_machine.h"

command_status do_badgerloop(int argc, char *argv[]) {

	int ret = 0;

	if (argc == 1) return USAGE;

	switch (argv[1][0]) {
	/* test dashboard functionality */
	case 'd':
		ret = get_performanceIV(send_telemetry_to_Dashboard);
		break;
	/* test SpaceX telemetry */
	case 's':
		//ret = get_performanceIV(send_telemetry_to_SpaceX);
		if (argc != 3) return USAGE;
		switch (argv[2][0]) {
		case 'i': state_handle.next_state = IDLE; break;
		case 'f': state_handle.next_state = FAULT; break;
		case 'r': state_handle.next_state = READY; break;
		case 'p': state_handle.next_state = PUSHING; break;
		case 'c': state_handle.next_state = COAST; break;
		case 'b': state_handle.next_state = BRAKING; break;
		default: return USAGE;
		}
		state_handle.change_state = true;
		break;
	/* perform DAQ / update values */
	case 'r':
		get_performanceVV(badgerloop_update_data);
		break;
	case 'm':
		if (argc < 3) return USAGE;
		ret = send_message_to_Dashboard(argv[2], strlen(argv[2]));
		break;
	case 'p':
		set_stdio_target(UDP);
		break;
	case 'q':
		ret = get_performanceIV(query_Dashboard);
		break;
	default: return USAGE;
	}
	printf("returned %d\r\n", ret);
	return SUCCESS;
}
COMMAND_ENTRY("badgerloop", "badgerloop <init>", "Debug Badgerloop Networking etc.", do_badgerloop)

