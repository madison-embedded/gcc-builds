#include "badgerloop.h"
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "cli.h"
#include "state_machine.h"

command_status do_badgerloop(int argc, char *argv[]) {

	int ret = 0;

	if (argc == 1) return USAGE;

	/* actuation functions */
	if (argc == 3) {
		if (!strcmp(argv[1], "pbrake"))
			primary_brakes(!strcmp(argv[2], "on") ? 100 : 0);
		else if (!strcmp(argv[1], "sbrake"))
			secondary_brakes(!strcmp(argv[2], "on") ? 100 : 0);
		else if (!strcmp(argv[1], "vpbrake"))
			vent_primary_brakes(!strcmp(argv[2], "on"));
		else if (!strcmp(argv[1], "vsbrake"))
			vent_secondary_brakes(!strcmp(argv[2], "on"));
		else if (!strcmp(argv[1], "thrust"))
			thrusters(!strcmp(argv[2], "on"));
		else if (!strcmp(argv[1], "vthrust"))
			vent_thrusters(!strcmp(argv[2], "on"));
		else if (!strcmp(argv[1], "state")) {
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
		} else return USAGE;
		return SUCCESS;
	}

	switch (argv[1][0]) {
	/* test dashboard functionality */
	case 'd':
		ret = get_performanceIV(send_telemetry_to_Dashboard);
		break;
	/* test SpaceX telemetry */
	case 's':
		ret = get_performanceIV(send_telemetry_to_SpaceX);
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
	case 'l':
		printf("PLIM1:\t%s\r\n", GET_PLIM1 ? "Open" : "Pressed");
		printf("PLIM2:\t%s\r\n", GET_PLIM2 ? "Open" : "Pressed");
		printf("BLIM1:\t%s\r\n", GET_BLIM1 ? "Pressed" : "Open");
		printf("BLIM2:\t%s\r\n", GET_BLIM2 ? "Pressed" : "Open");
		printf("DLIM:\t%s\r\n", GET_DLIM ? "Open" : "Pressed");
		break;
	default: return USAGE;
	}
	printf("returned %d\r\n", ret);
	return SUCCESS;
}
COMMAND_ENTRY("badgerloop", "badgerloop <init>", "Debug Badgerloop Networking etc.", do_badgerloop)

