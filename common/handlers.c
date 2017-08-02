#include <stdio.h>
#include "timer.h"
#include "config.h"

volatile uint8_t interrupt_test_mode = 1; 
/* can be used to roughly keep track of time, implement timeouts and blocking delays */
/* this 32-bit count overflows after 49.7 days */
volatile unsigned int ticks = 0;
void SysTick_Handler(void) { ticks++; }

void fault(void) {
	
	setLED(0, true);

	printf("Entered fault!\r\n");
	printf("TODO: add arguments to this function to trace fault.\r\n");
	
	while (1) {
		setLED(1, true);
		delay_ms(1000);
		setLED(1, false);
		delay_ms(1000);
	}
}

/* helpful: http://www.freertos.org/Debugging-Hard-Faults-On-Cortex-M-Microcontrollers.html */

/* I don't think any of these will actually work because
   the pending/active bits for these faults may need to
   be cleared in SCB->SHCSR. Need to design a test for this as well */

/* SCB is System Control Block (page 192) 
   SHCSR is System Handler Control and State Register (page 204 and 205). 
   SCB->SHCSR determines whether or not an interrupt is pending and 
   whether or not it is active. Set both pending and active bits to zero to
   clear the interrupt. 
*/

void NMI_Handler(void) {
	//SCB->SHCSR;
	SCB->ICSR &= ~(1<<31); //NMI-set-pending bit 
	if(interrupt_test_mode != 1)
	{
		fault();
	}
	else
	{
		printf("NMI_Handler test checked\n");
	}
	
}

void HardFault_Handler(void) {
	//SCB->SHCSR;
	//SCB->HFSR;

	SCB->HFSR &= ~(1<<30 | 1<<1);
	if(interrupt_test_mode != 1)
	{	
		fault();
	}
	else
	{
		printf("HardFault_Handler test checked\n");
	}
}

void MemManage_Handler(void) {
	//SCB->SHCSR;
	//SCB->MMFAR; contains the address where the MemManage fault occurs
	//SCB->CFSR;

	//NVIC_ClearPendingIRQ(MemoryManagement_IRQn);

	//set to be not active and not pending
	SCB->SHCSR &= ~(1<<13 | 1<<0); //page 204 and 205 
	//page 206
	SCB->CFSR &= ~(1<<7 | 1<<5 | 1<<4 | 1<<3 | 1<<1 | 0x1); //value in MMAR is not a valid fault address 
	if(interrupt_test_mode != 1)
	{
		fault();
	}
	else
	{
		printf("MemManage_Handler test checked \n");
	}
}

void BusFault_Handler(void) {
	//SCB->SHCSR;
	//SCB->BFAR; contains the address where the BusFault occurs
	//SCB->CFSR;

	//NVIC_ClearPendingIRQ(BusFault_IRQn);
	//set to not active and not pending
	SCB->SHCSR &= ~(1<<14 | 1<<1);  //page 204 and 205 
	//page 207 and 208
	SCB->CFSR &= ~(1<<15 | 1<<13 | 1<<12 | 1<<11 | 1<<10 | 0x9 | 0x8);
	if(interrupt_test_mode != 1)
	{
		fault();
	}
	else
	{
		printf("BusFault_Handler test checked \n");
	}
}

void UsageFault_Handler(void) {
	//SCB->SHCSR;
	//SCB->USFR;
	//SCB->CFSR;

	//NVIC_ClearPendingIRQ(UsageFault_IRQn);
	SCB->SHCSR &= ~(1<<12 | 1<<3);
	//page 209 
	SCB->CFSR &= ~(1<<25 | 1<<24 | 1<<19 | 1<<28 | 1<<17 | 1<<16);  
	if(interrupt_test_mode != 1)
	{
		fault();
	}
	else
	{
		printf("UsageFault_Handler test checked \n");
	}
}

