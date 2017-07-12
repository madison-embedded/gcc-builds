#include "gpio.h"

#define FILL_AFIO(_name, _port, _pin, _mode, _af_val, _speed, _pullup, _usable) \
	 {\
		.name = _name,\
		.port = _port,\
		.pin = _pin,\
		.mode = _mode,\
		.speed = _speed,\
		.pullup = _pullup,\
		.usable = _usable,\
		.af_val = _af_val\
	},

#define FILL_GPIO(_name, _port, _pin, _mode, _speed, _pullup, _usable) \
	FILL_AFIO(_name, _port, _pin, _mode, 0, _speed, _pullup, _usable)

#define NUM_GPIO_ALIAS sizeof(GPIO_TABLE)/sizeof(GPIO)

/* TODO: function that initializes pins and checks duplicates */
const GPIO GPIO_TABLE[] = {
	
	/* STM32F767ZI */
	FILL_GPIO("JTDI",			GPIOA, 15, ALT, LOW_SPEED, PULL_UP, false)
	FILL_GPIO("SWCLK",			GPIOA, 14, ALT, VERY_HIGH_SPEED, PULL_DOWN, false)
	FILL_GPIO("SWDAT",			GPIOA, 13, ALT, LOW_SPEED, NONE, false)
	FILL_GPIO("NJTRST",			GPIOB,  4, ALT, LOW_SPEED, PULL_UP, false)
	FILL_GPIO("JTDO",			GPIOB,  3, ALT, VERY_HIGH_SPEED, NONE, false)
	FILL_GPIO("OSC_IN",			GPIOH,  0, INPUT, LOW_SPEED, NONE, false)
	FILL_GPIO("OSC_OUT",		GPIOH,  1, INPUT, LOW_SPEED, NONE, false)
	FILL_GPIO("LSE_IN",			GPIOC, 14, INPUT, LOW_SPEED, NONE, false)
	FILL_GPIO("LSE_OUT",		GPIOC, 15, INPUT, LOW_SPEED, NONE, false)

	/* Nucleo 144 I/O */
	FILL_GPIO("REDLED",			GPIOB, 14, OUTPUT, LOW_SPEED, NONE, true)
	FILL_GPIO("BLUELED",		GPIOB,  7, OUTPUT, LOW_SPEED, NONE, true)
	FILL_GPIO("GREENLED",		GPIOB,  0, OUTPUT, LOW_SPEED, NONE, true)
	FILL_GPIO("BUTTON",			GPIOC, 13,  INPUT, LOW_SPEED, NONE, true)

	/* Nucleo 144 USB USART */
	FILL_AFIO("UART TX",		GPIOD, 8, ALT, 7, HIGH_SPEED, NONE, true)
	FILL_AFIO("UART RX",		GPIOD, 9, ALT, 7, HIGH_SPEED, NONE, true)

	/*	Nucleo 144 USB */
	FILL_GPIO("USB SOF",		GPIOA,  8, INPUT, LOW_SPEED, NONE, false)
	FILL_GPIO("USB VBUS",		GPIOA,  9, INPUT, LOW_SPEED, NONE, false)
	FILL_GPIO("USB ID",			GPIOA, 10, INPUT, LOW_SPEED, NONE, false)
	FILL_GPIO("USB DM",			GPIOA, 11, INPUT, LOW_SPEED, NONE, false)
	FILL_GPIO("USB DP",			GPIOA, 12, INPUT, LOW_SPEED, NONE, false)
	FILL_GPIO("USB GPIO OUT",	GPIOG,  6, INPUT, LOW_SPEED, NONE, false)
	FILL_GPIO("USB GPIO IN",	GPIOG,  7, INPUT, LOW_SPEED, NONE, false)

	/* Nucleo 144 Ethernet */
	FILL_GPIO("RMII REFCLK",	GPIOA, 1, OUTPUT, LOW_SPEED, NONE, false)
	FILL_GPIO("RMII MDIO",		GPIOA, 2, OUTPUT, LOW_SPEED, NONE, false)
	FILL_GPIO("RMII MDC",		GPIOC, 1, OUTPUT, LOW_SPEED, NONE, false)
	FILL_GPIO("RMII RX DV",		GPIOA, 7, OUTPUT, LOW_SPEED, NONE, false)
	FILL_GPIO("RMII RXD0",		GPIOC, 4, OUTPUT, LOW_SPEED, NONE, false)
	FILL_GPIO("RMII RXD1",		GPIOC, 5, OUTPUT, LOW_SPEED, NONE, false)
	FILL_GPIO("RMII TX EN",		GPIOG, 11, OUTPUT, LOW_SPEED, NONE, false)
	FILL_GPIO("RMII TXD0",		GPIOG, 13, OUTPUT, LOW_SPEED, NONE, false)
	FILL_GPIO("RMII TXD1",		GPIOB, 13, OUTPUT, LOW_SPEED, NONE, false)

	/* Application I/O */

	/* Application Analog */

	/* Application PWM */

	/* Application Input Capture */
	FILL_GPIO("Retro 1",		GPIOE,  9, ALT, LOW_SPEED, NONE, false)
	FILL_GPIO("Retro 2",		GPIOE, 11, ALT, LOW_SPEED, NONE, false)
	FILL_GPIO("Retro 3",		GPIOE, 13, ALT, LOW_SPEED, NONE, false)

	/* Application I2C */
	FILL_GPIO("I2C2 SDA",		GPIOF, 0, ALT, LOW_SPEED, NONE, false)
	FILL_GPIO("I2C2 SCL",		GPIOF, 1, ALT, LOW_SPEED, NONE, false)

	/* Application SDMMC */
	FILL_GPIO("SDMMC1 D0",		GPIOC,  8, ALT, LOW_SPEED, NONE, false)
	FILL_GPIO("SDMMC1 D1",		GPIOC,  9, ALT, LOW_SPEED, NONE, false)
	FILL_GPIO("SDMMC1 D2",		GPIOC, 10, ALT, LOW_SPEED, NONE, false)
	FILL_GPIO("SDMMC1 D3",		GPIOC, 11, ALT, LOW_SPEED, NONE, false)
	FILL_GPIO("SDMMC1 CK",		GPIOC, 12, ALT, LOW_SPEED, NONE, false)
	FILL_GPIO("SDMMC1 CMD",		GPIOD,  2, ALT, LOW_SPEED, NONE, false)
};

