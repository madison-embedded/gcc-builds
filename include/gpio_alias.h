#include "gpio.h"

#define FILL_GPIO(_name, _port, _pin, _mode, _speed, _pullup, _usable) \
	 {\
		.name = _name,\
		.port = _port,\
		.pin = _pin,\
		.mode = _mode,\
		.speed = _speed,\
		.pullup = _pullup,\
		.usable = _usable,\
	},
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

	/* Nucleo 144 LEDs */
	FILL_GPIO("REDLED",			GPIOB, 14, OUTPUT, LOW_SPEED, NONE, true)
	FILL_GPIO("BLUELED",		GPIOB,  7, OUTPUT, LOW_SPEED, NONE, true)
	FILL_GPIO("GREENLED",		GPIOB,  0, OUTPUT, LOW_SPEED, NONE, true)
	FILL_GPIO("BUTTON",			GPIOC, 13,  INPUT, LOW_SPEED, NONE, true)

	/* Nucleo 144 USB USART */
	FILL_GPIO("UART TX",		GPIOD, 8, ALT, HIGH_SPEED, NONE, true)
	FILL_GPIO("UART RX",		GPIOD, 9, ALT, HIGH_SPEED, NONE, true)

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

	/* Application I2C */
	FILL_GPIO("I2C2 SDA",		GPIOF, 0, ALT, MEDIUM_SPEED, NONE, false)
	FILL_GPIO("I2C2 SCL",		GPIOF, 1, ALT, MEDIUM_SPEED, NONE, false)

	/* Application SDMMC */
};

