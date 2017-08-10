#include "badgerloop.h"
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "cli.h"

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
		ret = get_performanceIV(start_query_Dashboard);
		break;
	default: return USAGE;
	}
	printf("returned %d\r\n", ret);
	return SUCCESS;
}
COMMAND_ENTRY("badgerloop", "badgerloop <init>", "Debug Badgerloop Networking etc.", do_badgerloop)

