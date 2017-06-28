#include <stdio.h>
#include <stdlib.h>
#include <cli.h>
#include "config.h"

command_status do_reset(int argc, char *argv[]){
	if (argc>1){
		printf("reset not implemented\r\n");
		return USAGE;
	}	
	NVIC_SystemReset();
	
}

COMMAND_ENTRY("reset","reset","do a software reset",do_reset)

