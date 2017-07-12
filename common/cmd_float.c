
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "cli.h"
float result;
float a[1];
float b[1];
int clocks_per_sec = 16000000;
double currTicks1,currTicks2;



command_status do_fpcal(int argc, char *argv[]){
	double ticks_spent;
	if ((argc < 4) | (strcmp(argv[0], "float") !=0 )) return USAGE;
	if (strcmp(argv[1], "+") ==0 | strcmp(argv[1], "add") ==0  ){ 
		*a = atof(argv[2]);
		*b = atof(argv[3]);
		currTicks1 = SysTick->VAL;
		result = a[0] + b[0];
		currTicks2 = SysTick->VAL;
		printf("Result= %.8f\r\n", result);
		ticks_spent = currTicks1 - currTicks2;
		printf ("TimeElapsed(ticks) = %.8f\r\n", ticks_spent);

	}

	else if (strcmp(argv[1], "-") ==0 | strcmp(argv[1], "abstract") ==0){
		*a = atof(argv[2]);
		*b = atof(argv[3]);
		currTicks1 = SysTick -> VAL;
		result = a[0] - b[0];
		currTicks2 = SysTick -> VAL;
		ticks_spent = currTicks1 - currTicks2;
		printf("Result= %.8f\r\n", result);
		printf ("TimeElapsed(ticks) = %.8f\r\n", ticks_spent); 	
	}

	else if (strcmp(argv[1], "*") ==0 | strcmp(argv[1], "multiply") ==0){
	
		*a = atof(argv[2]);
		*b = atof(argv[3]);
		currTicks1 = SysTick -> VAL;
        	result = a[0] * b[0];
		currTicks2 = SysTick -> VAL;
		ticks_spent = currTicks1 - currTicks2;
		printf("Result= %.8f\r\n", result);
		printf ("TimeElapsed(ticks) = %.8f\r\n", ticks_spent); 
		
	}
	else if (strcmp(argv[1], "/") ==0 | strcmp(argv[1], "divide") ==0){
	
		*a = atof(argv[2]);
 		*b = atof(argv[3]);
		currTicks1 = SysTick -> VAL; 	
		result = a[0] / b[0];
		currTicks2 = SysTick -> VAL;
		ticks_spent = currTicks1 - currTicks2;
		printf("Result= %.8f\r\n", result);
		printf ("TimeElapsed(ticks) = %.8f\r\n", ticks_spent); 
	}
	

	return SUCCESS;

}
 
COMMAND_ENTRY("float", "float [ add | subtract | multiply | divide | + | - | * | / ] <arg1> <arg2>", "Float point calculation using FPU.", do_fpcal)
