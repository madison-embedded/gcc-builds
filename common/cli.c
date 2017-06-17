#include <stdio.h>
#include <string.h>
#include "config.h"
#include "pcbuffer.h"
#include "cli.h"

char buffer[BUFSIZ];

inline void printPrompt(void) {
	printf("=> ");
	fflush(stdout);
}

command_status do_help(int argc, char *argv[]) {
	printf("help not implemented");
	return SUCCESS;
}

extern command_status do_md(int argc, char *argv[]);
extern command_status do_mw(int argc, char *argv[]);
extern command_status do_boot(int argc, char *argv[]);

const command_t commands[] = {
	COMMAND_ENTRY("help", "", "", do_help)
	COMMAND_ENTRY("md", "md <addr> <count>", "View raw memory contents.", do_md)
	COMMAND_ENTRY("mw", "mw <addr> <value> <count>", "Write data memory contents.", do_mw)
	COMMAND_ENTRY("boot", "boot <routine>", "Run ", do_boot)
};

int get_command_index(char *command) {
	int i;

	printf("num commands: %d\r\n", sizeof(commands)/sizeof(command_t));

	for (i = 0; i < sizeof(commands)/sizeof(command_t); i++) {
		printf("comparing %s and %s\r\n", command, commands[i].name);
		if (!strncmp(command, commands[i].name, strlen(commands[i].name))) {
			printf("found!!\r\n");
			return i;
		}
	}

	printf("function failed\r\n");
	
	while(1) {;}

	return -1;
}

void check_input(void) {
	unsigned int i, args_index = 0, initial_buf_len, command_index;
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

			/* temporary */
			printf("%d arguments given: ", args_index);
			for (i = 0; i < args_index; i++)
				printf("%s, ", args[i]);
			printf("\r\n");

			/* search argv[0] for matching command name */
			command_index = get_command_index(args[0]);

			printf("%d back\r\n", command_index);

			if (command_index >= 0) {
				result = commands[command_index].fp(args_index, args);
				if (result == FAIL)
					printf("%s failed\r\n", buffer);
				if (result == USAGE)
					printf("%s usage: %s\r\n", buffer, commands[command_index].usage);
			}
			else
				printf("unknown command: %s\r\n", buffer);
		}
		printPrompt();
	}
}

