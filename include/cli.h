#ifndef _CLI__H__
#define _CLI__H__

#define MAX_ARGS	10

typedef enum {
	SUCCESS = 0,
	USAGE,
	FAIL
} command_status;

typedef struct command {
	const char *name;
	const char *usage;
	const char *help;
	command_status (*fp)(int argc, char *argv[]);
	
} command_t;

void printPrompt(void);
void check_input(void);

#define COMMAND_ENTRY(_name, _usage, _help, _fp)\
	{\
		.name = _name,\
		.usage = _usage,\
		.help = _help,\
		.fp = _fp\
	},

#endif

