#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "gpio.h"

command_status gpio(int argc, char *argv[]) {
    
    if (argc < 4) return USAGE;
    char * func = argv[1];
    char port = argv[3][0];
    uint8_t pin;

    if (strcmp(func, "print") == 0){
    char * pinOrPort = argv[2];

        if (strcmp(pinOrPort, "pin") == 0){
            pin = atoi((const char *)&argv[3][1]);
            if (argc < 4) return USAGE;
            gpio_printPinInfo(getGpioPort(port), pin);
        }else if (strcmp(pinOrPort, "port") == 0){
               int i;
            for (i=0; i<16; i++)
            gpio_printPinInfo(getGpioPort(port), i);
      
        }else return USAGE;


    }else if (strcmp(func,"set") == 0){
    if (argc < 4) return USAGE;
    char port = argv[2][0];
    pin = atoi((const char *)&argv[2][1]);
    char * onOrOff = argv[3];
    GPIO_MODE mode = gpio_getMode(getGpioPort(port), pin);
        if (strcmp(onOrOff, "on")==0){
                   if (mode == ALT || mode == ANALOG){
                printf("Pin is either alternate function or analog \r\n");
                return FAIL;
            }
            gpio_setMode(getGpioPort(port), pin, OUTPUT);
            gpio_writePin(getGpioPort(port), pin, 1);
        }else if (strcmp(onOrOff, "off")==0){
            if (mode == ALT || mode == ANALOG){
                printf("Pin is either alternate function or analog \r\n");
                return FAIL;
            }

            gpio_setMode(getGpioPort(port), pin, OUTPUT);
            gpio_writePin(getGpioPort(port), pin, 0);
        }else return USAGE;

 
    }else return USAGE;

       
    return SUCCESS;
}
COMMAND_ENTRY("gpio", "printPinInfo <port>[num] [info | on | off | read]", "GPIO configuration", gpio)


