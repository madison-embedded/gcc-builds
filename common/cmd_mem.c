#include <stdio.h>
#include "cli.h"

command_status do_md(int argc, char *argv[]) {
	printf("md not implemented\r\n");
	return USAGE;
}
COMMAND_ENTRY("md", "md <addr> <count>", "View raw memory contents.", do_md)

command_status do_mw(int argc, char *argv[]) {
	printf("mw not implemented\r\n");
	return USAGE;
}
COMMAND_ENTRY("mw", "mw <addr> <value> <count>", "Write data memory contents.", do_mw)

