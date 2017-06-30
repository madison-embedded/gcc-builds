#include "gpio.h"
#define FILL_GPIO(_name, _pin, _port, _mode, _speed, _pullup, _usable) \
	 {\
		.name = _name,\
		.pin = _pin,\
		.port = _port,\
		.mode = _mode,\
		.speed = _speed,\
		.pullup = _pullup,\
		.usable = _usable,\
	},
#define NUM_GPIO_ALIAS sizeof(GPIO_TABLE)/sizeof(GPIO)

const GPIO GPIO_TABLE[]={
	FILL_GPIO("PRESSURE1", 1, GPIOA, OUTPUT, MEDIUM_SPEED, NONE, true)
	FILL_GPIO("PRESSURE2", 2, GPIOA, OUTPUT, MEDIUM_SPEED, NONE, true)
	FILL_GPIO("PRESSURE3", 3, GPIOA, OUTPUT, MEDIUM_SPEED, NONE, true)
	FILL_GPIO("TEMP1", 4, GPIOA, OUTPUT, MEDIUM_SPEED, NONE, true)
	FILL_GPIO("TEMP2", 5, GPIOA, OUTPUT, MEDIUM_SPEED, NONE, true)
	FILL_GPIO("TEMP3", 6, GPIOA, OUTPUT, MEDIUM_SPEED, NONE, true)
	FILL_GPIO("REDLED", 14, GPIOB, OUTPUT, MEDIUM_SPEED, NONE, true)
	FILL_GPIO("BLUELED", 7, GPIOB, OUTPUT, MEDIUM_SPEED, NONE, true)
	FILL_GPIO("GREENLED", 0, GPIOB, OUTPUT, MEDIUM_SPEED, NONE, true)
	FILL_GPIO("ETHERNET", 0, GPIOC, OUTPUT, MEDIUM_SPEED, NONE, false)
};

