#include <stdio.h>
#include "timer.h"
#include "config.h"
#include "proc/defs.h"

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
extern I2C_HandleTypeDef hi2c;
/**
 * Ethernet IRQ Handler
 *
 * @param  None
 * @retval None
 */
void ETH_IRQHandler(void) {
    HAL_ETH_IRQHandler(&EthHandle);
}

void print_elapsed_time(uint64_t before_timer, uint64_t after_timer,
						uint64_t before_tick, uint64_t after_tick) {
	uint64_t clocks_elapsed, time_elapsed;
	/* count down timer */
	clocks_elapsed = before_timer - after_timer;
	if (before_tick != after_tick)
		clocks_elapsed += SystemCoreClock*(after_tick - before_tick);  

	/* @160MHz, 1 cycle = 6.25 ns */
	// TODO: calculate clocks / nano-second manually, potentially
	// use a float here
	time_elapsed = (clocks_elapsed * 6) + (clocks_elapsed / 4);

	if (time_elapsed > 1000000)
		printf("Took %lluM clock cycles (%llu ms)\r\n",
			clocks_elapsed/1000000, time_elapsed/1000000);
	else if (time_elapsed > 1000)
		printf("Took %lluk clock cycles (%llu us)\r\n",
			clocks_elapsed/1000, time_elapsed/1000);
	else
		printf("Took %llu clock cycles (%llu ns)\r\n",
			clocks_elapsed, time_elapsed);
}

/* void void */
void get_performanceVV(void (*func)(void)) {

	uint64_t before, after, before_tick, after_tick;

	before_tick = ticks;
	before = SysTick->VAL;
	__disable_irq();
	func();
	__enable_irq();
	after = SysTick->VAL;
	after_tick = ticks;

	print_elapsed_time(before, after, before_tick, after_tick);
}

/* int void */
int get_performanceIV(int (*func)(void)) {

	uint64_t before, after, before_tick, after_tick;
	int retval;

	before_tick = ticks;
	before = SysTick->VAL;
	__disable_irq();
	retval = func();
	__enable_irq();
	after = SysTick->VAL;
	after_tick = ticks;

	print_elapsed_time(before, after, before_tick, after_tick);

	return retval;
}

uint16_t get_performanceITI(uint16_t(*func)(ADC_TypeDef *, uint8_t), ADC_TypeDef * adc, uint8_t pin) {

	uint64_t before, after, before_tick, after_tick;
	uint16_t retval;

	before_tick = ticks;
	before = SysTick->VAL;
	__disable_irq();
	retval = func(adc, pin);
	__enable_irq();
	after = SysTick->VAL;
	after_tick = ticks;

	print_elapsed_time(before, after, before_tick, after_tick);

	return retval;
}

void I2C2_EV_IRQHandler(void)
{
  HAL_I2C_EV_IRQHandler(&hi2c);
}

void I2C2_ER_IRQHandler(void)
{
  HAL_I2C_ER_IRQHandler(&hi2c);
}
