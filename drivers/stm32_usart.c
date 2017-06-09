#include "usart.h"

PC_Buffer usart3_tx, usart3_rx;

static IRQn_Type uart_get_irq_num(USART_TypeDef* usart) {
   switch((uint32_t) usart) {
     case USART1_BASE: return USART1_IRQn;
     case USART2_BASE: return USART2_IRQn;
     case USART3_BASE: return USART3_IRQn;
     case UART4_BASE:  return UART4_IRQn;
     case UART5_BASE:  return UART5_IRQn;
     case USART6_BASE: return USART6_IRQn;
     case UART7_BASE:  return UART7_IRQn;
     default: return SysTick_IRQn;	/* -1 */
   }
}

static int usart_bufferInit(USART_TypeDef* usart) {
	switch ((uint32_t) usart) {
	//case USART1_BASE: break;
	//case USART2_BASE: break;
	case USART3_BASE:
		if (!pc_buffer_init(&usart3_tx, BUFSIZ))
			return -1;
		if (!pc_buffer_init(&usart3_rx, BUFSIZ))
			return -1;
		break;
	//case UART4_BASE: break;
	//case UART5_BASE: break;
	//case USART6_BASE: break;
	//case UART7_BASE: break;
	//case UART8_BASE: break;
	default: return -1;	
	}
	return 0;
}

static int usart_enableClock(USART_TypeDef* usart) {
	
	__IO uint32_t *reg;
	uint8_t bit;
	
	switch ((uint32_t) usart) {
	case USART1_BASE: reg = &RCC->APB2ENR; bit = RCC_APB2ENR_USART1EN_Pos; break;
	case USART2_BASE: reg = &RCC->APB1ENR; bit = RCC_APB1ENR_USART2EN_Pos; break;
	case USART3_BASE: reg = &RCC->APB1ENR; bit = RCC_APB1ENR_USART3EN_Pos; break;
	case UART4_BASE:  reg = &RCC->APB1ENR; bit = RCC_APB1ENR_UART4EN_Pos;  break;
	case UART5_BASE:  reg = &RCC->APB1ENR; bit = RCC_APB1ENR_UART5EN_Pos;  break;
	case USART6_BASE: reg = &RCC->APB2ENR; bit = RCC_APB2ENR_USART6EN_Pos; break;
	case UART7_BASE:  reg = &RCC->APB1ENR; bit = RCC_APB1ENR_UART7EN_Pos;  break;
	case UART8_BASE:  reg = &RCC->APB1ENR; bit = RCC_APB1ENR_UART8EN_Pos;  break;
	default: return -1;
	}
	
	*reg |= 0x1 << bit;
	
	return 0;
}

static int usart_setClockSource(USART_TypeDef* usart, USART_CLK_SRC src) {
	
	uint8_t pin_num = 0;
	
	if (src > 3) return -1;
	
	switch ((uint32_t) usart) {
		case USART1_BASE: pin_num =  0; break;
		case USART2_BASE: pin_num =  2; break;
		case USART3_BASE: pin_num =  4; break;
		case UART4_BASE:  pin_num =  6; break;
		case UART5_BASE:  pin_num =  8; break;
		case USART6_BASE: pin_num = 10; break;
		case UART7_BASE:  pin_num = 12; break;
		case UART8_BASE:  pin_num = 14; break;
		default: return -1;
	}
	
	RCC->DCKCFGR2 &= ~(0x3 << pin_num);
	RCC->DCKCFGR2 |= src << pin_num;
	
	return 0;
}

int usart_config(USART_TypeDef* usart, USART_CLK_SRC src, uint32_t control[3], uint32_t baud, bool ie) {
	
	uint32_t usartDiv, fck, remainder;
	
	/* turn this USART off if it's on */
	if (usart->CR1 & 0x1) {
		usart->CR1 &= ~0x1;
		while (usart->CR1 & 0x1) {;}
	}
	
	if (usart_setClockSource(usart, src) || usart_bufferInit(usart)) return -1;
	
	usart_enableClock(usart);
	
	usart->ICR |= 0x21Bf5;	/* clear any pending interrupts */
	
	if (control) {
		usart->CR1 = control[0] & ~(0xD);	/* don't set TE, RE, UE yet */
		usart->CR2 = control[1];
		usart->CR3 = control[2];
	}
	
	if (ie) {
		NVIC_SetPriority(uart_get_irq_num(usart), 4);
		NVIC_EnableIRQ(uart_get_irq_num(usart));
	}
	
	/* setup baud, fck / USARTDIV */
	switch (src) {
		case APB1:		
			fck = APB1_F; 
			break;
		case SYSCLK:	
			fck = SystemCoreClock;
			break;
		case HSI_SRC:	/* check if enabled */
			if (!(RCC->CR & RCC_CR_HSION)) return -1;
			fck = HSI_VALUE; break;
		case LSE_SRC:	/* check if enabled */
			if (!(RCC->BDCR & RCC_BDCR_LSEON)) return -1;
			fck = LSE_VALUE; break;
	}
	
	remainder = fck % baud;
	usartDiv = fck / baud;
	
	if (remainder > baud / 2) usartDiv++;
	
	/* not sure how to handle OVER8 being set */
	usart->BRR = usartDiv;
	
	usart->CR1 |= 0xD; 		/* set TE, RE, UE */
	
	return 0;
}

void USART3_IRQHandler(void) {
	
	static char prev = '\0', curr = '\0';
	
	/* character received */
	if (USART3->ISR & USART_ISR_RXNE) {
		prev = curr;
		curr = USART3->RDR;
		
		/* backspace */
		if (curr == 0x08 || curr == 0x7F) {
            if (!pc_buffer_empty(&usart3_rx)) {
				usart3_rx.produce_count--;
				
				/* delete the character in console */
				if (!pc_buffer_full(&usart3_tx)) {
					pc_buffer_add(&usart3_tx, curr);
					USART3->CR1 |= USART_CR1_TXEIE;
				}
			}
		}
		
		/* otherwise add the character */
        else {
            if (NEWLINE_GUARD) usart3_rx.message_available++;
            if (!pc_buffer_full(&usart3_rx)) pc_buffer_add(&usart3_rx, curr);
			if (!pc_buffer_full(&usart3_tx)) {
				pc_buffer_add(&usart3_tx, curr);
				if (curr == '\r') pc_buffer_add(&usart3_tx, '\n');
				USART3->CR1 |= USART_CR1_TXEIE;
			}
        }
		
		/* echo character to console */
		if (!pc_buffer_full(&usart3_tx)) {

		}
	}
	
	/* character ready to be sent */
	if (USART3->ISR & USART_ISR_TXE) {
		if (!pc_buffer_empty(&usart3_tx))
			pc_buffer_remove(&usart3_tx, (char *) &USART3->TDR);
		else
			USB_UART->CR1 &= ~USART_CR1_TXEIE;
	}
}
