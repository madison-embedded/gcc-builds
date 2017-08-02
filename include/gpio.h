#ifndef _GPIO__H__
#define _GPIO__H__

#include <stdint.h>
#include <stdbool.h>
#include "proc/defs.h"

/*****************************************************************************/
/*                                 GPIO Enums                                */
/*****************************************************************************/
typedef enum {
	INPUT =		0,
	OUTPUT =	1,
	ALT =		2,
	ANALOG =	3
} GPIO_MODE;

typedef enum {
	LOW_SPEED =			0,
	MEDIUM_SPEED =		1,
	HIGH_SPEED =		2,
	VERY_HIGH_SPEED =	3
} GPIO_SPEED;

typedef enum {
	NONE = 		0,
	PULL_UP =	1,
	PULL_DOWN =	2
} GPIO_PULLUP_STATE;

typedef enum {
	OTHER = 0x0,
	SENSOR = 0x1,
	SOLENOID = 0x2,
	RETRO = 0x5,
	LED = 0x8,
	I2C = 0x11,
	PRESSURE = 0x21
} GPIO_GROUP;

typedef struct _GPIO{
	GPIO_TypeDef* port;
	uint8_t pin;
	bool state;
	GPIO_MODE mode;
	unsigned char af_val;
	GPIO_SPEED speed;
	GPIO_PULLUP_STATE pullup;
	const char *name;
	bool usable;
	GPIO_GROUP group;
} GPIO;


/*****************************************************************************/
/*****************************************************************************/


/*****************************************************************************/
/*                                 Functions                                 */
/*****************************************************************************/

/*****************************************************************************/
/*                                  Setters                                  */
/*****************************************************************************/

int gpio_setClock(GPIO_TypeDef* port, bool state);
int gpio_setMode(GPIO_TypeDef* port, uint8_t pin, GPIO_MODE mode);
int gpio_openDrainState(GPIO_TypeDef* port, uint8_t pin, bool state);
int gpio_setSpeed(GPIO_TypeDef* port, uint8_t pin, GPIO_SPEED speed);
int gpio_setPullupState(GPIO_TypeDef* port, uint8_t pin, GPIO_PULLUP_STATE state);
int gpio_readPin(GPIO_TypeDef* port, uint8_t pin);
void gpio_setPin(GPIO_TypeDef* port, uint8_t pin);
void gpio_resetPin(GPIO_TypeDef* port, uint8_t pin);
int gpio_writePin(GPIO_TypeDef* port, uint8_t pin, bool state);
int gpio_setAlternateFunc(GPIO_TypeDef* port, uint8_t pin, uint8_t val);

/*****************************************************************************/
/*                                  Getters                                  */
/*****************************************************************************/

GPIO_TypeDef * gpio_getGpioPort(char portChar);
char gpio_getGpioPortChar(GPIO_TypeDef * port);

GPIO_MODE gpio_getMode(GPIO_TypeDef* port, uint8_t pin);
GPIO_SPEED gpio_getSpeed(GPIO_TypeDef* port, uint8_t pin);
GPIO_PULLUP_STATE gpio_getPullupState(GPIO_TypeDef* port, uint8_t pin);
bool gpio_getPinState(GPIO_TypeDef* port, uint8_t pin);
int gpio_getAlternateFunc(GPIO_TypeDef* port, uint8_t pin);
void gpio_getPinInfo(GPIO_TypeDef* port, uint8_t pin, GPIO * gpio);


/*****************************************************************************/
/*****************************************************************************/

#endif
