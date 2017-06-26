#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "gpio.h"

void gpio_printPinInfo(GPIO_TypeDef* port, uint8_t pin){
    printf("%c%2d \t", getGpioPortChar(port), pin);
    
    switch(gpio_getMode(port, pin)){
        case OUTPUT:
            printf("outputting %x ", gpio_readPin(port, pin));
            break;
        case INPUT:
            printf("Value %x ", gpio_readPin(port, pin));
            break;
        case ALT:
            printf("alternate TODO ");
            break;
        case ANALOG:
            printf("analog TODO ");
            break;
        default:
            printf("UNKNOWN     ");
    }
    printf("\t");

    printf("Speed: ");
    switch (gpio_getSpeed(port, pin)){
        case LOW_SPEED:
            printf("low speed");
            break;
        case MEDIUM_SPEED:
            printf("medium speed");
            break;
        case HIGH_SPEED:
            printf("high speed");
            break;
        case VERY_HIGH_SPEED:
            printf("very high");
            break;
        default: 
            printf("UNKNOWN   ");

    }
    printf("\t");

    printf("Pullup: ");
     switch (gpio_getPullupState(port, pin)){
        case NONE:
            printf("none");
            break;
        case PULL_UP:
            printf("up");
            break;
        case PULL_DOWN:
            printf("down");
            break;
        default: 
            printf("UNKNOWN");

    }
     printf("\r\n");

    
}

command_status pin(int argc, char *argv[]) {
    if (argc < 3) return USAGE;
    bool portOnly = 1;
    char portTemp = argv[1][0];
    GPIO_TypeDef * port = getGpioPort(portTemp);
    int pin;
    char * func = argv[2];
    
    if (strlen(argv[1])>1){
        pin = atoi((const char *) &argv[1][1]);
        portOnly = 0;

        if (pin > 15) return USAGE;
    }

    if (!IS_GPIO_ALL_INSTANCE(port) !=0 ) return USAGE;

    if (strcmp(func, "info") == 0){

        if (!portOnly){
            gpio_printPinInfo(port, pin);
        }else {
            int i;
            for (i=0; i<16; i++)
            gpio_printPinInfo(port, i);
        }


    }else if (strcmp(func,"on") == 0){
    GPIO_MODE mode = gpio_getMode(port, pin);
    if (mode == ALT || mode == ANALOG){
                printf("Pin is either alternate function or analog \r\n");
                return FAIL;
            }
            gpio_setMode(port, pin, OUTPUT);
            gpio_writePin(port, pin, 1);

 
    }else if (strcmp(func,"off") == 0){
    GPIO_MODE mode = gpio_getMode(port, pin);
            if (mode == ALT || mode == ANALOG){
                printf("Pin is either alternate function or analog \r\n");
                return FAIL;
            }

            gpio_setMode(port, pin, OUTPUT);
            gpio_writePin(port, pin, 0);
    }else return USAGE;

       
    return SUCCESS;
}
COMMAND_ENTRY("pin", "pin <port>[num] [info | on | off ]", "GPIO configuration", pin)


