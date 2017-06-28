#include <stdio.h>
#include "cli.h"
#include "config.h"

command_status do_reset(int argc, char *argv[]) {
	NVIC_SystemReset();
	return SUCCESS;
}

COMMAND_ENTRY("reset","reset","Software reset.",do_reset)

