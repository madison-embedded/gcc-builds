#ifndef _CLI__H__
#define _CLI__H__

typedef enum {
	SUCCESS = 0,
	USAGE,
	FAIL
} command_error;

typedef struct command {
	const char *name;
	const char *usage;
	const char *help;
	command_error (*fp)(int argc, char *argv[]);
	
} command_t;

command_error example_command(int argc, char *argv[]);

void printPrompt(void);
void processCommand(void);
void _ttywrch(int ch);

#define COMMAND_ENTRY(_name, _usage, _help, _fp) \
	{\
	 	.name = _name,\
		.usage = _usage,\
		.help = _help,\
		.fp = _fp\
	},

extern const command_t commands[];

#endif

