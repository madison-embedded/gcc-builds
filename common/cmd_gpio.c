#include <stdio.h>
#include "cli.h"
#include "gpio.h"

command_status printPinInfo(int argc, char *argv[]) {
	gpio_printPinInfo(GPIOB, 1);
    
    return USAGE;
}
COMMAND_ENTRY("printPinInfo", "printPinInfo <port> <pin>", "Print information about a GPIO Pin", printPinInfo)


