#include <stdio.h>
#include <string.h>
#include "config.h"
#include "pcbuffer.h"
#include "cli.h"

char buffer[BUFSIZ];
command_t *commands = (command_t *) &__COMMANDS_START;

void check_input(void) {
	unsigned int i, args_index = 0, initial_buf_len;
	int command_index;
	command_status result;
	char *args[MAX_ARGS];

	if (pc_buffer_getMessage(&USB_RX, buffer, BUFSIZ)) {

		if (buffer[0] != '\0') {

			/* tokenize input for argv */
			args[args_index++] = &buffer[0];
			initial_buf_len = strlen(buffer);
			for (i = 0; i < initial_buf_len; i++) {
				if (buffer[i] == ' ') {
					buffer[i] = '\0';
					args[args_index++] = &buffer[i + 1];
				}
				if (args_index >= MAX_ARGS) break;
			}

			/* search for matching command name */
			command_index = get_command_index(args[0]);

			if (command_index >= 0) {
				result = commands[command_index].fp(args_index, args);
				if (result == FAIL)
					printf("%s failed\r\n", buffer);
				if (result == USAGE)
					printf("%s usage: %s\r\n", buffer, commands[command_index].usage);
			}
			else
				printf("unknown command: '%s' - try 'help'\r\n", buffer);
		}
		printPrompt();
	}
}

inline void printPrompt(void) {
	printf("=> ");
	fflush(stdout);
}

int get_command_index(char *command) {
	int i;
	for (i = 0; i < NUM_COMMANDS; i++)
		if (!strncmp(command, commands[i].name, strlen(commands[i].name)))
			return i;
	return -1;
}

command_status do_help(int argc, char *argv[]) {
	int i, command_index;
	if (argc == 1)
		for (i = 0; i < NUM_COMMANDS; i++)
			printf("%s\t- %s\r\n", commands[i].name, commands[i].help);
	else {
		command_index = get_command_index(argv[1]);
		if (command_index >= 0) 
			printf("%s\r\n", commands[command_index].help);
		else
			printf("'%s' not a command\r\n", argv[1]);
	}
	return SUCCESS;
}
COMMAND_ENTRY("help", "help <command_name>", "Display a command's help message.", do_help)

