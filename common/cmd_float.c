#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "cli.h"

typedef enum {
	ADD, SUBTRACT, MULTIPLY, DIVIDE, ERROR
} FP_OP;

command_status do_fpcal(int argc, char *argv[]) {
	uint64_t time_elapsed = 0;
	volatile uint64_t before = 0, after = 0;
	double result = 0.0, a = 0.0, b = 0.0;
	FP_OP op = ERROR;

	if (argc < 4) return USAGE;

	a = atof(argv[1]);
	b = atof(argv[3]);

	if (a == 0.0) {
		printf("Error: could not parse argument %s as a float.\r\n", argv[1]);
		return USAGE;
	}
	else if (b == 0.0) {
		printf("Error: could not parse argument %s as a float.\r\n", argv[3]);
		return USAGE;
	}

	if (!strcmp(argv[2], "+") || !strcmp(argv[1], "add"))
		op = ADD;
	else if (!strcmp(argv[2], "-") || !strcmp(argv[2], "subtract"))
		op = SUBTRACT;
	else if (!strcmp(argv[2], "*") || !strcmp(argv[2], "multiply"))
		op = MULTIPLY;
	else if (!strcmp(argv[2], "/") || !strcmp(argv[2], "divide"))
		op = DIVIDE;

	switch (op) {
		case ADD:
			before = SysTick->VAL;
			result = a + b;
			after = SysTick->VAL;
			break;
		case SUBTRACT:
			before = SysTick->VAL;
			result = a - b;
			after = SysTick->VAL;
			break;
		case MULTIPLY:
			before = SysTick->VAL;
			result = a * b;
			after = SysTick->VAL;
			break;
		case DIVIDE:
			before = SysTick->VAL; 	
			result = a / b;
			after = SysTick->VAL;
			break;
		case ERROR:
			printf("Error: could not parse arguments.\r\n");
			return USAGE;
			break;
	}

	/* check for timer overflow (timer counts down) */
	if (after > before)
		time_elapsed = before * (ticks * (uint64_t) 16000) - after * (ticks * (uint64_t) 16000);
	else time_elapsed = before - after;

	printf("Result: %.8f\r\n", result);
	printf("Took %llu clock cycles (%llu ns)\r\n", time_elapsed, (time_elapsed * 6) + (time_elapsed / 4)); 

	return SUCCESS;
}
 
COMMAND_ENTRY("float", "float <arg1> [ add | subtract | multiply | divide | + | - | * | / ] <arg2>", "Float point calculation using FPU.", do_fpcal)

