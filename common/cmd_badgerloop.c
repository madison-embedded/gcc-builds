#include "badgerloop.h"
#include <stdio.h>
#include "cli.h"

command_status do_badgerloop(int argc, char *argv[]) {

	int ret = 0;

	if (argc == 1)
		return USAGE;
	switch (argv[1][0]) {
	/* test init */
	case 'i': ret = badgerloop_init(); break;
	/* test dashboard functionality */
	case 'd': ret = send_telemetry_to_Dashboard(); break;
	/* test SpaceX telemetry */
	case 's': ret = send_telemetry_to_SpaceX(); break;
	}
	printf("returned %d\r\n", ret);
	return SUCCESS;
}
COMMAND_ENTRY("badgerloop", "badgerloop <init>", "Debug Badgerloop Networking etc.", do_badgerloop)

