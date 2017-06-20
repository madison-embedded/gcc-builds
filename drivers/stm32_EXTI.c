

static IRQn_Type exti_get_irq_num(EXTI_TypeDef* exti) {
   switch((uint32_t) exti) {
     case USART1_BASE: return EXTI0_IRQn;
     case USART2_BASE: return EXTI1_IRQn;
     case USART3_BASE: return EXTI2_IRQn;
     case UART4_BASE:  return EXTI3_IRQn;
     case UART5_BASE:  return EXTI4_IRQn;
     case USART6_BASE: return EXTI9_5_IRQn;
     case USART6_BASE: return EXTI15_10_IRQn;
     default: return SysTick_IRQn;	/* -1 */
   }
}

int exti_config(EXTI_TypeDef* exti, UINT32 pin  bool ie) {
    exti->IMR /* interrupt mask register */
    exti->EMR /* event mask register */
    exti->RTSR /* rising trigger selection register */
    exti->FTSR /*falling trigger selection register */
    exti->SWIER /* software interrupt event register */
    exti->PR /* pending register */

    if (ie) {
		NVIC_SetPriority(exti_get_irq_num(usart), 4);
		NVIC_EnableIRQ(exti_get_irq_num(usart));
	}
}


void EXTI0_IRQHandler(void){
}

void EXTI1_IRQHandler(void){
}

void EXTI2_IRQHandler(void){
}

void EXTI3_IRQHandler(void){
}

void EXTI4_IRQHandler(void){
}

void EXTI9_5_IRQHandler(void){
}

void EXTI15_10_IRQHandler(void){
}
