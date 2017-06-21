int count;

static IRQn_Type exti_get_irq_num(uint32_t pin) {
   switch(pin) {
     case 0: return EXTI0_IRQn;
     case 1: return EXTI1_IRQn;
     case 2: return EXTI2_IRQn;
     case 3:  return EXTI3_IRQn;
     case 4:  return EXTI4_IRQn;
     case 5 ... 9: return EXTI9_5_IRQn;
     case 10 ... 15: return EXTI15_10_IRQn;
     default: return 0xffff;	/* -1 */
   }
}

int exti_config(uint32_t pin,  bool ie) {
    count= 0;

    EXTI->IMR = EXTI_IMR_MR3_Msk; /* interrupt mask register */
    EXTI->RTSR = EXTI_RTSR_TR3_Pos; /* rising trigger selection register */
    EXTI->FTSR = EXTI_FTSR_TR3_Msk;/*falling trigger selection register */
  
    SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI3_PA; 	/* SYSCFG external interrupt configuration registers */
    
    if (ie) {
		NVIC_SetPriority(exti_get_irq_num(pin), 4);
		NVIC_EnableIRQ(exti_get_irq_num(pin));
	}
}



void getNumInter(void){

printf("number of interrupts = %d\n", count);
}





void EXTI0_IRQHandler(void){
    printf("interrupted from pin 1 port A\n");
    count ++;
    EXTI->PR=EXTI_PR_PR0;
}

void EXTI1_IRQHandler(void){
    EXTI->PR=EXTI_PR_PR1;

}


void EXTI2_IRQHandler(void){
    EXTI->PR=EXTI_PR_PR2;

}

void EXTI3_IRQHandler(void){
    count ++;
    EXTI->PR=EXTI_PR_PR3;

}

void EXTI4_IRQHandler(void){
    EXTI->PR=EXTI_PR_PR4;
}

void EXTI9_5_IRQHandler(void){
    if (EXTI->PR & EX TI_PR_PR5)
        EXTI->PR=EXTI_PR_PR5;
    if (EXTI->PR & EX TI_PR_PR6)
        EXTI->PR=EXTI_PR_PR6;
    if (EXTI->PR & EX TI_PR_PR7)
        EXTI->PR=EXTI_PR_PR7;
    if (EXTI->PR & EX TI_PR_PR8)
        EXTI->PR=EXTI_PR_PR8;
    if (EXTI->PR & EX TI_PR_PR9)
        EXTI->PR=EXTI_PR_PR9;

}

void EXTI15_10_IRQHandler(void){
   if (EXTI->PR & EX TI_PR_PR10)
        EXTI->PR=EXTI_PR_PR10;
    if (EXTI->PR & EX TI_PR_PR11)
        EXTI->PR=EXTI_PR_PR11;
    if (EXTI->PR & EX TI_PR_PR12)
        EXTI->PR=EXTI_PR_PR12;
    if (EXTI->PR & EX TI_PR_PR13)
        EXTI->PR=EXTI_PR_PR13;
    if (EXTI->PR & EX TI_PR_PR14)
        EXTI->PR=EXTI_PR_PR14;
    if (EXTI->PR & EX TI_PR_PR15)
        EXTI->PR=EXTI_PR_PR15;
}
