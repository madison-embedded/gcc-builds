#include "main.h"

int main(void) {

	while(1) {

	}
}

uint32_t SystemCoreClock = 16000000;
const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
const uint8_t APBPrescTable[8] = {0, 0, 0, 0, 1, 2, 3, 4};

#define VECT_TAB_OFFSET  0x00 /* must be a multiple of 0x200 */
void SystemInit(void) {

#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
	SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
#endif

	RCC->CR |= (uint32_t) 0x00000001;	/* Set HSION bit */
	RCC->CFGR = 0x00000000;				/* Reset CFGR register */
	RCC->CR &= (uint32_t) 0xFEF6FFFF;	/* Reset HSEON, CSSON and PLLON bits */
	RCC->PLLCFGR = 0x24003010;			/* Reset PLLCFGR register */
	RCC->CR &= (uint32_t) 0xFFFBFFFF;	/* Reset HSEBYP bit */
	RCC->CIR = 0x00000000;				/* Disable all interrupts */

#ifdef VECT_TAB_SRAM
	SCB->VTOR = RAMDTCM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM */
#else
	SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */
#endif
}

