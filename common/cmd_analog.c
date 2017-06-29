#include <stdio.h>
#include "cli.h"

command_status do_ar(int argc, char *argv[]) {
	printf("ar not implemented\r\n");
	return USAGE;
}
COMMAND_ENTRY("ar", "ar <pin>", "Perform ADC conversions on available pins.", do_ar)

