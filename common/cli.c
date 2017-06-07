#include <stdio.h>
#include "config.h"
#include "pcbuffer.h"

char buffer[BUFSIZ];

inline void printPrompt(void) {
	printf("=> ");
}

void processCommand(void) {
	if (pc_buffer_getMessage(&USB_RX, buffer, BUFSIZ)) {
		if (buffer[0] != '\0') {
			/* need to pull the characters out of the buffer */
			printf("Got: %s\r\n", buffer);
		}
		printPrompt();
	}
}
