#include <stdbool.h>
#include "gpio.h"
#include "gpio_alias.h"
#include "usart.h"
#include "config.h"
#include "rcc.h"
#include "hal/stm32f7xx_hal.h"
#include "i2c.h"
#include "mpu9250.h" 


void setup_osc(void) {

	/* clear PLL_ON, PLLIS2_ON, PLLSAI_ON, HSE_ON */
 	rcc_setClk(PLL, false); rcc_setClk(HSE, false);
	rcc_setClk(PLLI2S, false); rcc_setClk(PLLSAI, false);
	
	/* configure PLLs */
	RCC->PLLCFGR = 0x24003010;	/* set PLL bits to default	*/
		
	/* Set PLL configs */
	RCC->PLLCFGR |= PLLN_VAL; RCC->PLLCFGR |= PLLM_VAL;
	RCC->PLLCFGR |= PLLP_VAL; RCC->PLLCFGR |= PLLQ_VAL;
	RCC->PLLCFGR |= PLLR_VAL;
		
	/* Clear out default bits, if you clear them first the board gets messed up */
	/* (bonus points if you can answer why that is) */
	RCC->PLLCFGR &= ~PLLN_VAL; RCC->PLLCFGR &= ~PLLM_VAL;
	RCC->PLLCFGR &= ~PLLP_VAL; RCC->PLLCFGR &= ~PLLQ_VAL;
	RCC->PLLCFGR &= ~PLLR_VAL;
		
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
		
	/* enable high-speed external oscillator and PLL */
 	rcc_setClk(HSE, true); rcc_setClk(PLL, true);
		
	/* switch to PLL as system clock, disable high-speed internal */
	rcc_changeSysClockSrc(PLL); rcc_setClk(HSI, false);
		
	/* update core clock variable */
	SystemCoreClockUpdate();
		
	/* setup SysTick timer, 1ms interrupts */
	SysTick_Config(SystemCoreClock / 1000);
}

/* static initializations that don't fail */
extern void print_post_info(void);
void early_init(void) {

	uint32_t init_regs[3] = {0, 0, 0};

	HAL_Init();

	setup_osc();

	gpioAliasInit();
	
	init_regs[0] = USART_CR1_RXNEIE;
	usart_config(USB_UART, SYSCLK, init_regs, DEBUG_BAUD, true);

	print_post_info();
}

/* instantiate UART & button + LEDs no matter what */
extern void printPrompt(void);
extern void lwip_init(void);
bool board_init(void) {
//	int16_t a[3];
	uint8_t a[10];
//	int b;	
//	float * dest = ( float * ) malloc(sizeof(float));
	early_init();

	gpio_openDrainState(GPIOF, 0, true);
	gpio_openDrainState(GPIOF, 1, true);

	/* TODO: I2C */
	I2C_init(I2C2_BASE);
	
	I2C_setSlaveAddr(0x70, I2C2_BASE);
//	b = readByte(0x68, WHO_AM_I_MPU9250);
//	printf("IM a = %x ", b);
//	initAK8963(dest,I2C2_BASE);
//	resetMPU9250();
// 	initMPU9250(I2C2_BASE);
	//initAK8963(dest,I2C2_BASE);
			
//	MPU9250SelfTest(dest);
//	readGyroData(a);
//	printf("x = %d, y = %d , z = %d deviation = %f\r\n", a[0], a[1],a[2], *dest);

	a[0]= 0x21; 
	I2C_Master_Transmit_Data(I2C2_BASE, 1, a);
	a[0] = 0xA0;
	I2C_Master_Transmit_Data(I2C2_BASE, 1, a);
	a[0] = 0xEF;
	I2C_Master_Transmit_Data(I2C2_BASE, 1, a);
	a[0] = 0x80;
	I2C_Master_Transmit_Data(I2C2_BASE, 1, a);
	a[0] = 0x00;
	a[1] = 0x3F;
	writeByte(0x70, a[0], a[1]);
	a[2] = 0x02;
	a[3] = 0x06;

	writeByte(0x70, a[2], a[3]);
	a[4] = 0x04;
	a[5] = 0x00;

	writeByte(0x70, a[4], a[5]);
	a[6] = 0x06;
	a[7] = 0x66;

	writeByte(0x70, a[6], a[7]);
	a[8] = 0x08;
	a[9] = 0x5B;

	writeByte(0x70, a[8], a[9]);

	I2C_Master_Transmit_Data(I2C2_BASE, 10, a);
	
	a[0] = 0x81;
	I2C_Master_Transmit_Data(I2C2_BASE, 1, a);

	lwip_init();

	printPrompt();
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

