#include <stdio.h>
#include "timer.h"
#include "config.h"

/* can be used to roughly keep track of time, implement timeouts and blocking delays */
/* this 32-bit count overflows after 49.7 days */
volatile unsigned int ticks = 0;
void SysTick_Handler(void) { ticks++; }
uint32_t HAL_GetTick(void) { return ticks; }

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

void assert_failed(uint8_t* file, uint32_t line) {
	fault();
}

/* helpful: http://www.freertos.org/Debugging-Hard-Faults-On-Cortex-M-Microcontrollers.html */

/* I don't think any of these will actually work because
   the pending/active bits for these faults may need to
   be cleared in SCB->SHCSR. Need to design a test for this as well */

void NMI_Handler(void) {
	//SCB->SHCSR;
	fault();
}

void HardFault_Handler(void) {
	//SCB->SHCSR;
	//SCB->HFSR;
	fault();
}

void MemManage_Handler(void) {
	//SCB->SHCSR;
	//SCB->MMFAR;
	//SCB->CFSR;
	fault();
}

void BusFault_Handler(void) {
	//SCB->SHCSR;
	//SCB->BFAR;
	//SCB->CFSR;
	fault();
}

void UsageFault_Handler(void) {
	//SCB->SHCSR;
	//SCB->USFR;
	//SCB->CFSR;
	fault();
}

#include "hal/stm32f7xx_hal.h"
extern ETH_HandleTypeDef EthHandle;

/**
 * Ethernet IRQ Handler
 *
 * @param  None
 * @retval None
 */
void ETH_IRQHandler(void) {
    HAL_ETH_IRQHandler(&EthHandle);
}

/* void void */
void get_performanceVV(void (*func)(void)) {

	uint64_t before = 0, after = 0, clocks_elapsed, time_elapsed;

	before = SysTick->VAL;
	__disable_irq();
	func();
	__enable_irq();
	after = SysTick->VAL;

	/* check for timer overflow (timer counts down) */
	if (after > before)
		clocks_elapsed = before * (ticks * (uint64_t) 16000) - after * (ticks * (uint64_t) 16000);
	else clocks_elapsed = before - after;

	time_elapsed = (clocks_elapsed * 6) + (clocks_elapsed / 4);

	/* @160MHz, 1 cycle = 6.25 ns */
	printf("Took %llu clock cycles (%llu ns)\r\n",
		clocks_elapsed, time_elapsed);
}

/* int void */
int get_performanceIV(int (*func)(void)) {

	uint64_t before = 0, after = 0, clocks_elapsed, time_elapsed;
	int retval;

	before = SysTick->VAL;
	__disable_irq();
	retval = func();
	__enable_irq();
	after = SysTick->VAL;

	/* check for timer overflow (timer counts down) */
	if (after > before)
		clocks_elapsed = before * (ticks * (uint64_t) 16000) - after * (ticks * (uint64_t) 16000);
	else clocks_elapsed = before - after;

	time_elapsed = (clocks_elapsed * 6) + (clocks_elapsed / 4);

	/* @160MHz, 1 cycle = 6.25 ns */
	printf("Took %llu clock cycles (%llu ns)\r\n",
		clocks_elapsed, time_elapsed);
	return retval;
}

