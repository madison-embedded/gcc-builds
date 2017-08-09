#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "config.h"
#include "pcbuffer.h"
#include "cli.h"

char buffer[BUFSIZ];

/* how to print a backspace to the console */
char backspace[3] = {0x08, ' ', 0x08};

/* flags for up arrow and down arrow behavior */
volatile bool upArrowFlag = false, downArrowFlag = false;
bool swapReady = false;

/* reference to command table */
command_t *commands = (command_t *) &__COMMANDS_START;

void process_input(char *buffer) {
	unsigned int initial_buf_len, args_index = 0;
	int command_index, i;
	command_status result;
	char *args[MAX_ARGS];

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

void check_input(void) {
	int i; char temp;

	/* erase current contents of line */
	if (upArrowFlag || downArrowFlag) {
		while (!pc_buffer_empty(&USB_RX)) {
			__disable_irq();
			pc_buffer_remove(&USB_RX, &temp);
			__enable_irq();
			_write(0, backspace, 3);
		}
		downArrowFlag = false;
	}

	/* go back to previous command */
	if (upArrowFlag) {
		if (swapReady) {
			for (i = 0; i < strlen(buffer); i++)
				pc_buffer_add(&USB_RX, buffer[i]);
			printf("%s", buffer);
			fflush(stdout);
			swapReady = false;
		}
		upArrowFlag = false;
	}

	else if (pc_buffer_messageAvailable(&USB_RX)) {
		pc_buffer_getMessage(&USB_RX, buffer, USART_BUF);
		if (buffer[0] != '\0') {
			swapReady = true;
			process_input(buffer);
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
			printf("%s\r\nUsage: %s\r\n", commands[command_index].help,
					commands[command_index].usage);
		else
			printf("'%s' not a command.\r\n", argv[1]);
	}
	return SUCCESS;
}
COMMAND_ENTRY("help", "help [command_name]", "Display a command's help message.", do_help)

