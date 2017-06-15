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

command_error test1(int argc, char *argv[]) {
	printf("ran test1\r\n");
	return SUCCESS;
}

command_error test2(int argc, char *argv[]) {
	printf("ran test2\r\n");
	return SUCCESS;
}

const command_t commands[] = {
	COMMAND_ENTRY("test1", "test1 usage", "test1 help", test1)
	COMMAND_ENTRY("test2", "test2 usage", "test2 help", test2)
};

void processCommand(void) {
	unsigned int i;
	command_error result;

	if (pc_buffer_getMessage(&USB_RX, buffer, BUFSIZ)) {

		if (buffer[0] != '\0') {

			/* need to parse buffer into an array of arguments */

			for (i = 0; i < sizeof(commands)/sizeof(command_t); i++) {
				if (!strcmp(buffer, commands[i].name)) {
					result = commands[i].fp(1, (char**) &buffer);
					if (result == FAIL)
						printf("%s failed\r\n", buffer);
					if (result == USAGE)
						printf("%s usage: %s\r\n", buffer, commands[i].usage);
					break;
				}
			}

			if (i == sizeof(commands)/sizeof(command_t))
				printf("unknown command: %s\r\n", buffer);

		}
		printPrompt();
	}
}

