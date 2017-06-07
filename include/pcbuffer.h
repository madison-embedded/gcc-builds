#ifndef __PC_BUFFER_H__
#define __PC_BUFFER_H__

bool pc_buffer_init(PC_Buffer *buffer, uint16_t buffer_size);
void pc_buffer_add(PC_Buffer *buffer, char data);
void pc_buffer_remove(PC_Buffer *buffer, char *data);
bool pc_buffer_empty(PC_Buffer *buffer);
bool pc_buffer_full(PC_Buffer *buffer);
bool pc_buffer_getMessage(PC_Buffer *buffer, char *message, int maxLength);

#endif
