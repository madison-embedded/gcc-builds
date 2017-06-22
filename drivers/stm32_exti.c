#include "exti.h"

#include "gpio.h"
/*****************************************************************************/
/*                        Welcome to Cooper's Code                           */
/*****************************************************************************/

int *count;

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
int exti_config(uint32_t pin, char port, bool rtsr, bool ftsr, bool ie) {
    count= malloc(sizeof(int)*16);
     int i;
     
     
    gpio_setMode(GPIOC, 1, INPUT);
      GPIOC->OSPEEDR |= (0x03<< (2 * pin));    // high speed
    RCC->APB2ENR |= (1 << 14); 
     
     
     
     
     
     printf("(port-65) << (pin mod 4) 0x%08x\r\n", (port-65) << (pin%4));
     printf("pin/4 %d port - 65 %d, pin mod 4 %d , total 0x%08x\r\n",pin/4, port - 65 ,pin%4, (port-65) << (pin%4));
    
    SYSCFG->EXTICR[pin/4] = (port-65) << (pin%4); 	/* SYSCFG external interrupt configuration registers */
    for (i=0;i<4;i++)
        printf("*(SYSCFG->EXTICR+4*%d) 0x%08x\r\n", i, SYSCFG->EXTICR[i]);
    
   


















    for (i=0; i<16; i++)
    {
        count[i] = 0;
    }   
EXTI->PR=0xffff;
    EXTI->IMR |= 0x1U << pin;
    
    if (rtsr)
        EXTI->RTSR |= 0x1U << pin;   
    else
        EXTI->RTSR &= ~(0x1U << pin);    /* rising trigger selection register */
    if (ftsr)
        EXTI->FTSR |= 0x1U << pin;       /*falling trigger selection register */
    else
        EXTI->FTSR &= ~(0x1U << pin);


printf("EXTI->IMR 0x%08x\r\n",  EXTI->IMR);
printf("EXTI->RTSR 0x%08x\r\n", EXTI->RTSR);
printf("EXTI->FTSR 0x%08x\r\n", EXTI->FTSR);



    if (ie) {
		NVIC_SetPriority(exti_get_irq_num(pin), 4);
		NVIC_EnableIRQ(exti_get_irq_num(pin));
	}
}

/*disable interrupt from given pin */
/* TODO */

/*gets number of interrupts from given pin*/
void getNumInter(int pin){
    int i;
for (i =0; i<16;i++)
printf("pin %d has %d interrupts\r\n", i, count[i]);
}



/*need to get time stamps*/
void EXTI0_IRQHandler(void){
    count[0] ++;
    EXTI->PR=EXTI_PR_PR0;
}

void EXTI1_IRQHandler(void){
    count[1] ++;
    EXTI->PR=EXTI_PR_PR1;
EXTI->PR=0xffff;

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
    if (EXTI->PR & EXTI_PR_PR5){
        count[5] ++;
        EXTI->PR=EXTI_PR_PR5;
    }
    if (EXTI->PR & EXTI_PR_PR6){
        count[6] ++;
        EXTI->PR=EXTI_PR_PR6;
    }
    if (EXTI->PR & EXTI_PR_PR7){
        count[7] ++;
        EXTI->PR=EXTI_PR_PR7;
    }
    if (EXTI->PR & EXTI_PR_PR8){
        count[8] ++;
        EXTI->PR=EXTI_PR_PR8;
    }
    if (EXTI->PR & EXTI_PR_PR9){
        count[9] ++;
        EXTI->PR=EXTI_PR_PR9;
    }
EXTI->PR=0xffff;

}

void EXTI15_10_IRQHandler(void){
   if (EXTI->PR & EXTI_PR_PR10){
        count[10] ++;
        EXTI->PR=EXTI_PR_PR10;
   }
    if (EXTI->PR & EXTI_PR_PR11){
        count[11] ++;
        EXTI->PR=EXTI_PR_PR11;
    }
    if (EXTI->PR & EXTI_PR_PR12){
        count[12] ++;
        EXTI->PR=EXTI_PR_PR12;
    }
    if (EXTI->PR & EXTI_PR_PR13){
        count[13] ++;
        EXTI->PR=EXTI_PR_PR13;
    }
    if (EXTI->PR & EXTI_PR_PR14){
        count[14] ++;
        EXTI->PR=EXTI_PR_PR14;
    }
    if (EXTI->PR & EXTI_PR_PR15){
        count[15] ++;
        EXTI->PR=EXTI_PR_PR15;
    }
EXTI->PR=0xffff;
}
