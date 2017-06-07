#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "config.h"

bool pc_buffer_init(PC_Buffer *buffer, uint16_t buffer_size) {
	void *temp = malloc(sizeof(char)*buffer_size);
	if (!temp) return false;
	buffer->array = temp;
	buffer->buffer_SIZE = buffer_size;
	buffer->consume_count = 0;
	buffer->produce_count = 0;
	buffer->message_available = 0;
	return true;
}

void pc_buffer_add(PC_Buffer *buffer, char data) {
	buffer->array[buffer->produce_count++ % buffer->buffer_SIZE] = data;
}

void pc_buffer_remove(PC_Buffer *buffer, char *data) {
	*data = buffer->array[buffer->consume_count++ % buffer->buffer_SIZE];
}

bool pc_buffer_empty(PC_Buffer *buffer) {
	return buffer->produce_count == buffer->consume_count;
}

bool pc_buffer_full(PC_Buffer *buffer) {
	return buffer->produce_count - buffer->consume_count == buffer->buffer_SIZE;
}

bool pc_buffer_getMessage(PC_Buffer *buffer, char *message, int maxLength) {
	char curr = '\0', prev = '\0';
	int index = 0;
	
	if (!buffer->message_available) return false;
	
	while (index < maxLength && !pc_buffer_empty(buffer)) {
		prev = curr;
		pc_buffer_remove(buffer, &curr);
		message[index++] = (NEWLINE_GUARD) ? '\0' : curr;
		if (NEWLINE_GUARD) {
			buffer->message_available--;
			return true;
		}
	}
	return false;
}
