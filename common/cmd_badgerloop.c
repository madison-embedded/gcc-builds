#include "badgerloop.h"
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "cli.h"
#include "state_machine.h"

command_status do_badgerloop(int argc, char *argv[]) {

	int ret = 0;

	if (argc == 1) return USAGE;

	if (argc == 3) {
		/* variable set functions */
		if (!strcmp("accel", argv[1])) {
			SET_ACCEL(strtol(argv[2], NULL, 10));
			return SUCCESS;
		} else if (!strcmp("position", argv[1])) {
			SET_POS(strtoul(argv[2], NULL, 10));
			return SUCCESS;
		} else if (!strcmp("velocity", argv[1])) {
			SET_VEL(strtoul(argv[2], NULL, 10));
			return SUCCESS;
		} else if (!strcmp("strip", argv[1])) {
			SET_SCOUNT(strtoul(argv[2], NULL, 10));
			return SUCCESS;
		} else if (!strcmp("bls", argv[1])) {
			SET_BRP1(strtoul(argv[2], NULL, 10));
			return SUCCESS;
		} else if (!strcmp("blp", argv[1])) {
			SET_BRP2(strtoul(argv[2], NULL, 10));
			return SUCCESS;
		} else if (!strcmp("bpp", argv[1])) {
			SET_BRP3(strtoul(argv[2], NULL, 10));
			return SUCCESS;
		} else if (!strcmp("pl", argv[1])) {
			SET_PRP1(strtoul(argv[2], NULL, 10));
			return SUCCESS;
		} else if (!strcmp("ps", argv[1])) {
			SET_PRP2(strtoul(argv[2], NULL, 10));
			return SUCCESS;
		}

		/* configurable fields */
		else if (!strcmp("DBTO", argv[1])) {
			DONT_BRAKE_TO = strtoul(argv[2], NULL, 10);
		} else if (!strcmp("MBTO", argv[1])) {
			MUST_BRAKE_TO = strtoul(argv[2], NULL, 10);
		} else if (!strcmp("BCT", argv[1])) {
			BRAKING_COUNT_THRS = strtoul(argv[2], NULL, 10);
		} else if (!strcmp("ACCEL", argv[1])) {
			ACCEL_IMPULSE = strtoul(argv[2], NULL, 10);
		} else if (!strcmp("TEP", argv[1])) {
			TARGET_END_POS = strtoul(argv[2], NULL, 10);
		} else if (!strcmp("CMPS", argv[1])) {
			CM_PER_STRIP = strtoul(argv[2], NULL, 10);
		}

		/* actuation functions */
		else if (!strcmp(argv[1], "pbrake"))
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
		} else if (!strcmp("override", argv[1])) {
			if (!strcmp("on", argv[2])) {
				printf("data override on\r\n");
				manual_update = true;
			} else if (!strcmp("off", argv[2])) {
				printf("data override off\r\n");
				manual_update = false;
			} else return FAIL;
		} else return USAGE;
		return SUCCESS;
	}

	switch (argv[1][0]) {
	/* test dashboard functionality */
#if NETWORKING
	case 'p':
		set_stdio_target(UDP);
		break;
	case 'q':
		ret = get_performanceIV(query_Dashboard);
		break;
	case 'd':
		ret = get_performanceIV(send_telemetry_to_Dashboard);
		break;
	/* test SpaceX telemetry */
	case 's':
		ret = get_performanceIV(send_telemetry_to_SpaceX);
		break;
#endif
	case 'c':
		printf("DONT_BRAKE_TO:\t\t%lu\r\n", DONT_BRAKE_TO);
		printf("MUST_BRAKE_TO:\t\t%lu\r\n", MUST_BRAKE_TO);
		printf("BRAKING_COUNT_THRS:\t%lu\r\n", BRAKING_COUNT_THRS);
		printf("ACCEL_IMPULSE:\t%lu\r\n", ACCEL_IMPULSE);
		printf("TARGET_END_POS:\t%lu\r\n", TARGET_END_POS);
		printf("CM_PER_STRIP:\t%lu\r\n", CM_PER_STRIP);
		break;
	/* perform DAQ / update values */
	case 'r':
		get_performanceVV(badgerloop_update_data);
		break;
	case 'm':
		if (argc < 3) return USAGE;
		ret = send_message_to_Dashboard(argv[2], strlen(argv[2]));
		break;
	case 'l':
		printf("PLIM1:\t%s\r\n", GET_PLIM1 ? "Open" : "Pressed");
		printf("PLIM2:\t%s\r\n", GET_PLIM2 ? "Open" : "Pressed");
		printf("BLIM1:\t%s\r\n", GET_BLIM1 ? "Open" : "Pressed");
		printf("BLIM2:\t%s\r\n", GET_BLIM2 ? "Open" : "Pressed");
		printf("DLIM:\t%s\r\n", GET_DLIM ? "Open" : "Pressed");
		break;
	case 'f':
		printf("%s: %d\r\n", fault_message, bad_value);
		break;
	default: return USAGE;
	}
	printf("returned %d\r\n", ret);
	return SUCCESS;
}
COMMAND_ENTRY("badgerloop", "badgerloop\
	DBTO  - Don't brake timeout\
	MBTO  - Must brake timeout\
	BCT   - Braking count threshold\
	ACCEL - Accelerometer impulse cap\
	TEP   - Target end position\
	CMPS  - Centimeters per strip\
	override [ on | off ] - stop DAQ and override sensor data\
	fault - print current fault message",
	"Debug Badgerloop Networking etc.", do_badgerloop)
