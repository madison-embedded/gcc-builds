#include <stdio.h>
#include "cli.h"

command_status do_boot(int argc, char *argv[]) {
	printf("boot not implemented\r\n");
	return USAGE;
}
COMMAND_ENTRY("boot", "boot <routine>", "Run currently selected main routine.", do_boot)

