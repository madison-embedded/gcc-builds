#include <stdbool.h>
#include "gpio.h"
#include "usart.h"
#include "config.h"
#include "rcc.h"
#include "hal/stm32f7xx_hal.h"

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
void early_init(void) {

	uint32_t init_regs[3] = {0, 0, 0};

	HAL_Init();

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
	usart_config(USB_UART, SYSCLK, init_regs, DEBUG_BAUD, true);
}

/* instantiate UART & button + LEDs no matter what */
bool board_init(void) {

	early_init();

	/* TODO: I2C */

	/* TODO: Ethernet */

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

void HAL_ETH_MspInit(ETH_HandleTypeDef* heth)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    if (heth->Instance == ETH) {

        /* Enable GPIOs clocks */
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOG_CLK_ENABLE();

        /** ETH GPIO Configuration
          RMII_REF_CLK ----------------------> PA1
          RMII_MDIO -------------------------> PA2
          RMII_MDC --------------------------> PC1
          RMII_MII_CRS_DV -------------------> PA7
          RMII_MII_RXD0 ---------------------> PC4
          RMII_MII_RXD1 ---------------------> PC5
          RMII_MII_RXER ---------------------> PG2
          RMII_MII_TX_EN --------------------> PG11
          RMII_MII_TXD0 ---------------------> PG13
          RMII_MII_TXD1 ---------------------> PB13
         */
        /* Configure PA1, PA2 and PA7 */
        GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStructure.Pull = GPIO_NOPULL; 
        GPIO_InitStructure.Alternate = GPIO_AF11_ETH;
        GPIO_InitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

        /* Configure PB13 */
        GPIO_InitStructure.Pin = GPIO_PIN_13;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

        /* Configure PC1, PC4 and PC5 */
        GPIO_InitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

        /* Configure PG2, PG11 and PG13 */
        GPIO_InitStructure.Pin =  GPIO_PIN_2 | GPIO_PIN_11 | GPIO_PIN_13;
        HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

        /* Enable the Ethernet global Interrupt */
        HAL_NVIC_SetPriority(ETH_IRQn, 0x7, 0);
        HAL_NVIC_EnableIRQ(ETH_IRQn);
        
        /* Enable ETHERNET clock  */
        __HAL_RCC_ETH_CLK_ENABLE();
    }
}

void HAL_ETH_MspDeInit(ETH_HandleTypeDef* heth)
{
    if (heth->Instance == ETH) {
        /* Peripheral clock disable */
        __HAL_RCC_ETH_CLK_DISABLE();

        /** ETH GPIO Configuration
          RMII_REF_CLK ----------------------> PA1
          RMII_MDIO -------------------------> PA2
          RMII_MDC --------------------------> PC1
          RMII_MII_CRS_DV -------------------> PA7
          RMII_MII_RXD0 ---------------------> PC4
          RMII_MII_RXD1 ---------------------> PC5
          RMII_MII_RXER ---------------------> PG2
          RMII_MII_TX_EN --------------------> PG11
          RMII_MII_TXD0 ---------------------> PG13
          RMII_MII_TXD1 ---------------------> PB13
         */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7);
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13);
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5);
        HAL_GPIO_DeInit(GPIOG, GPIO_PIN_2 | GPIO_PIN_11 | GPIO_PIN_13);

        /* Disable the Ethernet global Interrupt */
        NVIC_DisableIRQ(ETH_IRQn);
    }
}

