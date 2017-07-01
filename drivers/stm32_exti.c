#include "exti.h"

#define NUMBER_INTERUPT_PINS 16

typedef struct{
	int prev;
	int curr;
}timeStamp;

timeStamp * interLine;

/* gets interupt vecotor number for pin */
static IRQn_Type exti_get_irq_num(uint32_t pin) {
	switch(pin) {
		case 0: return EXTI0_IRQn;
		case 1: return EXTI1_IRQn;
		case 2: return EXTI2_IRQn;
		case 3:  return EXTI3_IRQn;
		case 4:  return EXTI4_IRQn;
		case 5 ... 9: return EXTI9_5_IRQn;
		case 10 ... 15: return EXTI15_10_IRQn;
		default: return SysTick_IRQn;	/* -1 */
	}
}

/*sets up an interrupt from a passed in port and pin */
void exti_config(GPIO_TypeDef * port, uint32_t pin, bool rtsr, bool ftsr, bool ie) {
	
	int i;
	interLine = malloc(sizeof(timeStamp)*NUMBER_INTERUPT_PINS); 
	for (i = 0; i < NUMBER_INTERUPT_PINS; i++) {
		interLine[i].prev = 0;
		interLine[i].curr = 0;
	}

	gpio_setMode(port, pin, INPUT);
	GPIOC->OSPEEDR |= (0x03<< (2 * pin));    // high speed
	RCC->APB2ENR |= (1 << 14); 

	SYSCFG->EXTICR[pin/4] |= (gpio_getGpioPortChar(port)-65) << (pin%4*4); 	/* SYSCFG external interrupt configuration registers */
	
	EXTI->PR=0xffff;
	EXTI->IMR |= 0x1U << pin;

	if (rtsr)
		EXTI->RTSR |= 0x1U << pin;		/* rising trigger selection register */
	else
		EXTI->RTSR &= ~(0x1U << pin);    	
	if (ftsr)
		EXTI->FTSR |= 0x1U << pin;       /* falling trigger selection register */
	else
		EXTI->FTSR &= ~(0x1U << pin);

	if (ie) {
		NVIC_SetPriority(exti_get_irq_num(pin), 4);
		NVIC_EnableIRQ(exti_get_irq_num(pin));
	}
}

/*disable interrupt from given pin */
void exti_disable(GPIO_TypeDef * port, uint32_t pin) {
		RCC->APB2ENR |= (1 << 14); 
		SYSCFG->EXTICR[pin/4] &= ~((gpio_getGpioPortChar(port)-65) << (pin%4*4)); 	/* SYSCFG external interrupt configuration registers */
		EXTI->IMR &= ~(0x1U << pin);
	}
/*gets number of interrupts from given pin*/
void getTimeStamps(int pin){
		printf("pin %d has prev %x curr %x \r\n", pin, interLine[pin].prev, interLine[pin].curr);
}

/*need to get time stamps*/
void EXTI0_IRQHandler(void) {
	if (EXTI->PR & EXTI_PR_PR0) {
		interLine[0].prev = interLine[0].curr;
		interLine[0].curr = ticks;
		EXTI->PR |= EXTI_PR_PR0;
	}
}

void EXTI1_IRQHandler(void) {
	if (EXTI->PR & EXTI_PR_PR1) {
		interLine[1].prev = interLine[1].curr;
		interLine[1].curr = ticks;
		EXTI->PR |= EXTI_PR_PR1;
	}
}

void EXTI2_IRQHandler(void) {
	if (EXTI->PR & EXTI_PR_PR2) {
		interLine[2].prev = interLine[2].curr;
		interLine[2].curr = ticks;
		EXTI->PR |= EXTI_PR_PR2;
	}
}

void EXTI3_IRQHandler(void) {
	if (EXTI->PR & EXTI_PR_PR3) {
		interLine[3].prev = interLine[3].curr;
		interLine[3].curr = ticks;
		EXTI->PR |= EXTI_PR_PR3;
	}
}

void EXTI4_IRQHandler(void) {
	if (EXTI->PR & EXTI_PR_PR4) {
		interLine[4].prev = interLine[4].curr;
		interLine[4].curr = ticks;
		EXTI->PR |= EXTI_PR_PR4;
	}
}

void EXTI9_5_IRQHandler(void) {
	if (EXTI->PR & EXTI_PR_PR5) {
		interLine[5].prev = interLine[5].curr;
		interLine[5].curr = ticks;
		EXTI->PR |= EXTI_PR_PR5;
	}
	if (EXTI->PR & EXTI_PR_PR6) {
		interLine[6].prev = interLine[6].curr;
		interLine[6].curr = ticks;
		EXTI->PR |= EXTI_PR_PR6;
	}
	if (EXTI->PR & EXTI_PR_PR7) {
		interLine[7].prev = interLine[7].curr;
		interLine[7].curr = ticks;
		EXTI->PR |= EXTI_PR_PR7;
	}
	if (EXTI->PR & EXTI_PR_PR8) {
		interLine[8].prev = interLine[8].curr;
		interLine[8].curr = ticks;
		EXTI->PR |= EXTI_PR_PR8;
	}
	if (EXTI->PR & EXTI_PR_PR9) {
		interLine[9].prev = interLine[9].curr;
		interLine[9].curr = ticks;
		EXTI->PR |= EXTI_PR_PR9;
	}

}

void EXTI15_10_IRQHandler(void) {
	if (EXTI->PR & EXTI_PR_PR10) {
		interLine[10].prev = interLine[10].curr;
		interLine[10].curr = ticks;
		EXTI->PR |= EXTI_PR_PR10;
	}
	if (EXTI->PR & EXTI_PR_PR11) {
		interLine[11].prev = interLine[11].curr;
		interLine[11].curr = ticks;
		EXTI->PR|= EXTI_PR_PR11;
	}
	if (EXTI->PR & EXTI_PR_PR12) {
		interLine[12].prev = interLine[12].curr;
		interLine[12].curr = ticks;
		EXTI->PR |= EXTI_PR_PR12;
	}
	if (EXTI->PR & EXTI_PR_PR13) {
		interLine[13].prev = interLine[13].curr;
		interLine[13].curr = ticks;
		EXTI->PR|= EXTI_PR_PR13;
	}
	if (EXTI->PR & EXTI_PR_PR14) {
		interLine[14].prev = interLine[14].curr;
		interLine[14].curr = ticks;
		EXTI->PR |= EXTI_PR_PR14;
	}
	if (EXTI->PR & EXTI_PR_PR15) {
		interLine[15].prev = interLine[15].curr;
		interLine[15].curr = ticks;
		EXTI->PR |= EXTI_PR_PR15;
	}
}
