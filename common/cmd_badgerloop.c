#include "badgerloop.h"
#include <stdio.h>
#include "config.h"
#include "cli.h"

command_status do_badgerloop(int argc, char *argv[]) {

	int ret = 0;

	if (argc == 1)
		return USAGE;
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
	}
	printf("returned %d\r\n", ret);
	return SUCCESS;
}
COMMAND_ENTRY("badgerloop", "badgerloop <init>", "Debug Badgerloop Networking etc.", do_badgerloop)

