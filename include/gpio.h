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

/*
typedef struct{
    uint8_t pin;
    bool state;
    GPIO_MODE mode;
    GPIO_SPEED speed;
    GPIO_PULLUP_STATE pullup;

} GPIO;
 */

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
/*
GPIO_MODE gpio_getMode((GPIO_TypeDef* port, uint8_t pin);
GPIO_SPEED gpio_getSpeed(GPIO_TypeDef* port, uint8_t pin);
GPIO_PULLUP_STATE gpio_getPullupState(GPIO_TypeDef* port, uint8_t pin);
bool gpio_getPin(GPIO_TypeDef* port, uint8_t pin);
int gpio_getAlternateFunc(GPIO_TypeDef* port, uint8_t pin);
GPIO gpio_getPinInfo(GPIO_TypeDef* port, uint8_t pin);
*/

/*****************************************************************************/
/*                        Display Pin Information                            */
/*****************************************************************************/
/*
GPIO gpio_printPinInfo(GPIO_TypeDef* port, uint8_t pin);
GPIO gpio_printPortInfo(GPIO_TypeDef* port, uint8_t pin);
GPIO gpio_printAllPinInfo(GPIO_TypeDef* port, uint8_t pin);
*/

/*****************************************************************************/
/*****************************************************************************/

#endif
