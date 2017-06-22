#include "stm32_exti.h"
/*****************************************************************************/
/*                        Welcome to Cooper's Code                           */
/*****************************************************************************/

int count[16];

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
     default: return 0xffff;	/* -1 */
   }
}


/*sets up an interrupt from a passed in port and pin */
int exti_config(uint32_t pin, char port, bool ie) {
    count= malloc(int*16);

    EXTI->IMR = 0x1U << pin; /* interrupt mask register */
    EXTI->RTSR = pin /* rising trigger selection register */
    EXTI->FTSR = 0x1U << pin;/*falling trigger selection register */
  printf("port - 40 %d, pin %4 %d , total %d\n", port-40 ,pin%4, port-40 << pin%4);
    SYSCFG->EXTICR[[pin%4] = port-40 << pin%4; 	/* SYSCFG external interrupt configuration registers */
    
    if (ie) {
		NVIC_SetPriority(exti_get_irq_num(pin), 4);
		NVIC_EnableIRQ(exti_get_irq_num(pin));
	}
}

/*disable interrupt from given pin */
/* TODO */

/*gets number of interrupts from given pin*/
void getNumInter(int pin){

printf("pin %d has %d interrupts\n", pin, count[pin]);
}



/*need to get time stamps*/
void EXTI0_IRQHandler(void){
    printf("interrupted from pin 1 port A\n");
    count[0] ++;
    EXTI->PR=EXTI_PR_PR0;
}

void EXTI1_IRQHandler(void){
    count[1] ++;
    EXTI->PR=EXTI_PR_PR1;

}


void EXTI2_IRQHandler(void){
    count[2] ++;
    EXTI->PR=EXTI_PR_PR2;

}

void EXTI3_IRQHandler(void){
    count[3] ++;
    EXTI->PR=EXTI_PR_PR3;

}

void EXTI4_IRQHandler(void){
    count[4] ++;
    EXTI->PR=EXTI_PR_PR4;
}

void EXTI9_5_IRQHandler(void){
    if (EXTI->PR & EX TI_PR_PR5){
        count[5] ++;
        EXTI->PR=EXTI_PR_PR5;
    }
    if (EXTI->PR & EX TI_PR_PR6){
        count[6] ++;
        EXTI->PR=EXTI_PR_PR6;
    }
    if (EXTI->PR & EX TI_PR_PR7){
        count[7] ++;
        EXTI->PR=EXTI_PR_PR7;
    }
    if (EXTI->PR & EX TI_PR_PR8){
        count[8] ++;
        EXTI->PR=EXTI_PR_PR8;
    }
    if (EXTI->PR & EX TI_PR_PR9){
        count[9] ++;
        EXTI->PR=EXTI_PR_PR9;
    }

}

void EXTI15_10_IRQHandler(void){
   if (EXTI->PR & EX TI_PR_PR10){
        count[10] ++;
        EXTI->PR=EXTI_PR_PR10;
   }
    if (EXTI->PR & EX TI_PR_PR11){
        count[11] ++;
        EXTI->PR=EXTI_PR_PR11;
    }
    if (EXTI->PR & EX TI_PR_PR12){
        count[12] ++;
        EXTI->PR=EXTI_PR_PR12;
    }
    if (EXTI->PR & EX TI_PR_PR13){
        count[13] ++;
        EXTI->PR=EXTI_PR_PR13;
    }
    if (EXTI->PR & EX TI_PR_PR14){
        count[14] ++;
        EXTI->PR=EXTI_PR_PR14;
    }
    if (EXTI->PR & EX TI_PR_PR15){
        count[15] ++;
        EXTI->PR=EXTI_PR_PR15;
    }
}
