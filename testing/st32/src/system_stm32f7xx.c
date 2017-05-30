#include "stm32f7xx.h"

/* Uncomment if you need to relocate your vector Table in Internal SRAM. */
/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET  0x00 /* This value must be a multiple of 0x200. */

/* Default value of the External oscillator in Hz */
#if !defined  (HSE_VALUE) 
#define HSE_VALUE	((uint32_t)8000000)
#endif /* HSE_VALUE */

/* Value of the Internal oscillator in Hz*/
#if !defined  (HSI_VALUE)
#define HSI_VALUE	((uint32_t)16000000) 
#endif /* HSI_VALUE */

uint32_t SystemCoreClock = 16000000;
const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
const uint8_t APBPrescTable[8] = {0, 0, 0, 0, 1, 2, 3, 4};

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

void SystemCoreClockUpdate(void) {
	uint32_t tmp = 0, pllvco = 0, pllp = 2, pllsource = 0, pllm = 2;
  
	tmp = RCC->CFGR & RCC_CFGR_SWS;	/* Get SYSCLK source */

	switch (tmp) {

	/* HSI used as system clock source */
	case 0x00: SystemCoreClock = HSI_VALUE; break;

	/* HSE used as system clock source */
	case 0x04: SystemCoreClock = HSE_VALUE; break;

	case 0x08:  /* PLL used as system clock source */

		/* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N
		   SYSCLK = PLL_VCO / PLL_P */    
		pllsource = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) >> 22;
		pllm = RCC->PLLCFGR & RCC_PLLCFGR_PLLM;
      
		/* HSE used as PLL clock source */
		if (pllsource != 0)
			pllvco = (HSE_VALUE / pllm) *
			((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);

		/* HSI used as PLL clock source */
		else
			pllvco = (HSI_VALUE / pllm) *
			((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);      

		pllp = (((RCC->PLLCFGR & RCC_PLLCFGR_PLLP) >> 16) + 1 ) * 2;
		SystemCoreClock = pllvco / pllp;
		break;

	default: SystemCoreClock = HSI_VALUE; break;

	}

	/* Compute HCLK frequency */
	tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)];
	SystemCoreClock >>= tmp;
}

