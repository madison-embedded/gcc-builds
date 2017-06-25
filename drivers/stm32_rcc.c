#include <stdint.h>
#include <stdbool.h>
#include "proc/defs.h"
#include "rcc.h"

/* TODO: check if trying to clear system clock source? */
bool rcc_setClk(clk_src_t clk, bool state) {
	uint32_t set_mask, ready_mask, result;
	__IO uint32_t *reg;
	
	switch (clk) {
		
		case HSI:
			reg = &RCC->CR;
			set_mask = RCC_CR_HSION;
			ready_mask = RCC_CR_HSIRDY;
			break;
		
		case HSE:
			reg = &RCC->CR;
			set_mask = RCC_CR_HSEON;
			ready_mask = RCC_CR_HSERDY;
		
			/* if setting, set bypass bit first */
			if (state)
				*reg = (HSE_BYP) ? 
					(*reg | RCC_CR_HSEBYP) : 
					(*reg & ~RCC_CR_HSEBYP);
			break;
			
		case PLL:
			reg = &RCC->CR;
			set_mask = RCC_CR_PLLON;
			ready_mask = RCC_CR_PLLRDY;
			break;
		
		case PLLSAI:
			reg = &RCC->CR;
			set_mask = RCC_CR_PLLSAION;
			ready_mask = RCC_CR_PLLSAIRDY;
			break;
		
		case PLLI2S:
			reg = &RCC->CR;
			set_mask = RCC_CR_PLLI2SON;
			ready_mask = RCC_CR_PLLI2SRDY;
			break;
		
		case LSI:
			reg = &RCC->CSR;
			set_mask = RCC_CSR_LSION;
			ready_mask = RCC_CSR_LSIRDY;
			break;
		
		case LSE:
			reg = &RCC->BDCR;
			set_mask = RCC_BDCR_LSEON;
			set_mask = RCC_BDCR_LSERDY;
		
			/* if setting, set bypass bit first */
			if (state)
				*reg = (HSE_BYP) ? 
					(*reg | RCC_CR_HSEBYP) : 
					(*reg & ~RCC_CR_HSEBYP);
			break;
			
		default: return false;
	}
	
	/* set or clear the appropriate bit */
	*reg = (state) ?
		(*reg | set_mask) : 
		(*reg & ~set_mask);
	
	do {
		result = (state) ?
			(*reg & ready_mask) :	/* turn on  = wait for bit to be set	*/
			!(*reg & ready_mask);	/* turn off = wait for bit to cleared	*/
	} while(!result);
	
	return true;
}

int rcc_getClockState(clk_src_t clk) {
	switch (clk) {
		case HSI: return RCC->CR & RCC_CR_HSIRDY; break;
		case HSE: return RCC->CR & RCC_CR_HSERDY; break;
		case PLL: return RCC->CR & RCC_CR_PLLRDY; break;
		case PLLSAI: return RCC->CR & RCC_CR_PLLSAIRDY; break;
		case PLLI2S: return RCC->CR & RCC_CR_PLLI2SRDY; break;
		case LSI: return RCC->CSR & RCC_CSR_LSIRDY; break;
		case LSE: return RCC->BDCR & RCC_BDCR_LSERDY; break;
	}
	return 0;
}

bool rcc_changeSysClockSrc(clk_src_t clk) {
	uint32_t set_mask, ready_mask;

	if (!rcc_getClockState(clk))
		return false;
	
	switch (clk) {
		case HSI:
			set_mask = RCC_CFGR_SW_HSI;
			ready_mask = RCC_CFGR_SWS_HSI;
			break;
		case HSE:
			set_mask = RCC_CFGR_SW_HSE;
			ready_mask = RCC_CFGR_SWS_HSE;
			break;
		case PLL:
			set_mask = RCC_CFGR_SW_PLL;
			ready_mask = RCC_CFGR_SWS_PLL;
			break;
		default: return false;
	}
	RCC->CFGR |= set_mask;
	while (!(RCC->CFGR & ready_mask)) {;}
	return true;
}

clk_src_t rcc_get_SysClockSrc(void) {
	switch (RCC->CFGR & RCC_CFGR_SWS_Msk) {
		case RCC_CFGR_SWS_HSI: return HSI;
		case RCC_CFGR_SWS_HSE: return HSE;
		case RCC_CFGR_SWS_PLL: return PLL;
	}
	return PLLI2S; /* no good way of indicating error */
}

void rcc_setPLLs() {
	
}
