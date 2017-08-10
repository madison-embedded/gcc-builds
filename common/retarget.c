#include <stdio.h>
#include "config.h"
#include "pcbuffer.h"
#include "cli.h"
#include "badgerloop.h"

#define BLOCK	0

stdio_target tar = USB_SERIAL;

void set_stdio_target(stdio_target t) {
	tar = t;
}

int _write(int fd, const void *buf, size_t count) {
	/* UDP Port 3000 */
	if (tar == UDP) {
		if (send_message_to_Dashboard((char *) buf, count) == -1)
			tar = USB_SERIAL;
		return count;
	}
	/* USB Serial */
	for (fd = 0; fd < count; fd++) {
		if (pc_buffer_full(&usart3_tx)) {
			USB_UART->CR1 |= USART_CR1_TXEIE;
			while (pc_buffer_full(&usart3_tx)) {;}
		}
		__disable_irq();
		pc_buffer_add(&usart3_tx, *((char *) buf++));
		__enable_irq();
	}
	USB_UART->CR1 |= USART_CR1_TXEIE;
#if BLOCK
	while (!pc_buffer_empty(&usart3_tx)) {;}
#endif
	return count;
}

int _read(int fd, const void *buf, size_t count) {
	for (fd = 0; fd < count; fd++) {
		while (pc_buffer_empty(&usart3_rx)) {;}
		__disable_irq();
		pc_buffer_remove(&usart3_rx, (char *) buf++);
		__enable_irq();
	}
	return count;
}

