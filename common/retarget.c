#include <stdio.h>
#include "config.h"
#include "pcbuffer.h"

int _write(int fd, const void *buf, size_t count) {
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

