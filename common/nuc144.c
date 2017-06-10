#include <stdbool.h>
#include "gpio.h"
#include "usart.h"
#include "config.h"

void setup_osc(void) {

	/* clear PLL_ON, PLLIS2_ON, PLLSAI_ON, HSE_ON */
	RCC->CR &= ~(RCC_CR_PLLSAION | RCC_CR_PLLI2SON | RCC_CR_PLLON | RCC_CR_HSEON);
	
	/* wait for those to be unlocked */
	while (RCC->CR & (RCC_CR_PLLSAIRDY | RCC_CR_PLLI2SRDY | RCC_CR_PLLRDY | RCC_CR_HSERDY)) {;}
	
	/* configure PLLs */
	RCC->PLLCFGR = 0x24003010;	/* set PLL bits to default	*/
		
	/* Set PLL configs */
	RCC->PLLCFGR |= PLLN; RCC->PLLCFGR |= PLLM;
	RCC->PLLCFGR |= PLLP; RCC->PLLCFGR |= PLLQ;
	RCC->PLLCFGR |= PLLR;
		
	/* Clear out default bits, if you clear them first the board gets messed up */
	/* (bonus points if you can answer why that is) */
	RCC->PLLCFGR &= ~PLLN; RCC->PLLCFGR &= ~PLLM;
	RCC->PLLCFGR &= ~PLLP; RCC->PLLCFGR &= ~PLLQ;
	RCC->PLLCFGR &= ~PLLR;
		
	/* set HSE as PLL clock src	*/
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;			
		
	/* set microcontroller clock outputs */
	RCC->CFGR =  0x0;						/* make sure it is in reset state */
	RCC->CFGR |= 0x0 << RCC_CFGR_MCO2_Pos;	/* MCO2 set to SYSCLK */
	RCC->CFGR |= RCC_CFGR_MCO2PRE_Msk;		/* MCO2 prescalar set to 5 */
	RCC->CFGR |= RCC_CFGR_MCO1;				/* MCO1 set to PLL */
	RCC->CFGR |= RCC_CFGR_MCO1PRE_Msk;		/* MCO1 prescalar set to 5 */
	RCC->CFGR |= RCC_CFGR_RTCPRE_3;			/* RTC prescalar set to 8 */
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;		/* APB2 (high speed, can't exceed 90 MHz) divided by 2 */
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;		/* APB1 (low speed, can't exceed 45 MHz) clock divided by 4 */
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;		/* AHB prescalar set to 0 */
		
	/* enable high-speed external oscillator */
	RCC->CR |= RCC_CR_HSEBYP;					/* set HSE_BYP, nucleo 144 uses 8 MHz signal from onboard ST-Link */
	RCC->CR |= RCC_CR_HSEON;					/* set HSE_ON to enable this clock source */
	while (!(RCC->CR & (RCC_CR_HSERDY))) {;}	/* poll HSE_RDY */
		
	/* enable PLL */
	RCC->CR |= RCC_CR_PLLON;					/* set PLL_ON	*/
	while (!(RCC->CR & (RCC_CR_PLLRDY))) {;}	/* poll PLL_RDY	*/
		
	/* switch to PLL as system clock */
	RCC->CFGR |= RCC_CFGR_SW_PLL;				/* request switch to PLL		*/
	while (!(RCC_CFGR_SWS_PLL)) {;}				/* wait for PLL to be source	*/
		
	/* update core clock variable */
	SystemCoreClockUpdate();
		
	/* setup SysTick timer, 1ms interrupts */
	SysTick_Config(SystemCoreClock / 1000);
}

/* static initializations that don't fail */
void early_init(void) {

	uint32_t init_regs[3] = {0, 0, 0};

	setup_osc();
	
	/* LEDs */
	gpio_setClock(LED_GPIO, true);
	gpio_setMode(LED_GPIO, GREEN_PIN, OUTPUT);
	gpio_setMode(LED_GPIO, BLUE_PIN, OUTPUT);
	gpio_setMode(LED_GPIO, RED_PIN, OUTPUT);
	
	/* button (no need for pull-up or pull-down) */
	gpio_setClock(BUTTON_GPIO, true);
	gpio_setMode(BUTTON_GPIO, BUTTON_PIN, INPUT);	
	
	/* USART3 -> ST-LINKv2 -> USB virtual COM */
	gpio_setClock(USB_UART_GPIO, true);
	gpio_setSpeed(USB_UART_GPIO, USB_UART_TX, HIGH_SPEED);
	gpio_setSpeed(USB_UART_GPIO, USB_UART_RX, HIGH_SPEED);
	gpio_setMode(USB_UART_GPIO, USB_UART_TX, ALT);
	gpio_setMode(USB_UART_GPIO, USB_UART_RX, ALT);
	gpio_setAlternateFunc(USB_UART_GPIO, USB_UART_TX, 7); /* TODO: define this somewhere? */
	gpio_setAlternateFunc(USB_UART_GPIO, USB_UART_RX, 7); /* TODO: define this somewhere? */
	init_regs[0] = USART_CR1_RXNEIE;
	usart_config(USB_UART, HSI_SRC, init_regs, DEBUG_BAUD, true);
}

/* instantiate UART & button + LEDs no matter what */
bool board_init(void) {

	return true;
}

void setRed(bool state) {
	state ? gpio_setPin(GPIOB, RED_PIN) :
			gpio_resetPin(GPIOB, RED_PIN);
}

void setBlue(bool state) {
	state ? gpio_setPin(GPIOB, BLUE_PIN) :
			gpio_resetPin(GPIOB, BLUE_PIN);
}

void setGreen(bool state) {
	state ? gpio_setPin(GPIOB, GREEN_PIN) :
			gpio_resetPin(GPIOB, GREEN_PIN);
}

void setLEDs(bool state) {
	setRed(state);
	setBlue(state);
	setGreen(state);
}

void setLED(int index, bool state) {
	switch (index) {
		case 0: setRed(state); break;
		case 1: setBlue(state); break;
		case 2: setGreen(state); break;
	}
}

inline bool readButton(void) {
	return gpio_readPin(GPIOC, BUTTON_PIN);
}

/* experimenting with oscillator pins
 *
 * gpio_setClock(GPIOA, true);
 * gpio_setSpeed(GPIOA, 8, VERY_HIGH_SPEED);
 * gpio_setSpeed(GPIOC, 9, VERY_HIGH_SPEED);
 * gpio_setAlternateFunc(GPIOA, 8, 1);
 * gpio_setAlternateFunc(GPIOC, 9, 1);
 * gpio_setMode(GPIOA, 8, ALT);
 * gpio_setMode(GPIOC, 9, ALT);
 */

