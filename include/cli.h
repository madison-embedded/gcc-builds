#ifndef _CLI__H__
#define _CLI__H__

void printPrompt(void);
void check_input(void);
int get_command_index(char *command);
void process_input(char *buffer);

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

extern command_t *commands;

/* symbols need to be defined in the linker script */
extern unsigned int __COMMANDS_END, __COMMANDS_START;

#define MAX_ARGS	10
#define NUM_COMMANDS (((unsigned int) &__COMMANDS_END - (unsigned int) &__COMMANDS_START)/sizeof(command_t))

#define COMMAND_ENTRY(_name, _usage, _help, _fp)\
	const command_t __attribute__((section (".commands"))) cmd_##_fp = \
	{\
		.name = _name,\
		.usage = _usage,\
		.help = _help,\
		.fp = _fp\
	};

#endif

