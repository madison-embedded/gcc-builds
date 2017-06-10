#ifndef _NUC144__H__
#define _NUC144__H__

#include <stdbool.h>

bool board_init(void);
void fault(void);
void setRed(bool state);
void setBlue(bool state);
void setGreen(bool state);
void setLEDs(bool state);
void setLED(int index, bool state);
bool readButton(void);

/*****************************************************************************/
/*                           Board Specifics                                 */
/*****************************************************************************/
/* 11 GPIO ports (A -> K)
 *
 * Defaults:
 *
 * A15: pulled up						JTDI
 * A14: pulled down						JTCK-SWCLK
 * A13: pulled up		very high speed	JTMS-SWDIO
 *  B4: pulled up						NJTRST
 *  B3: 				very high speed	JTDO/T RACES WO
 */
 
/* TODO: don't define things this way */
#define MC01		PA8
#define MC02		PC9
#define USART3TX	PD8
#define	USART3RX	PD9

#define RED_PIN			14
#define BLUE_PIN		7
#define GREEN_PIN		0		/* can be configured for PA5 */
#define LED_GPIO		GPIOB

#define BUTTON_PIN		13		/* can be configured for PA0 */
#define BUTTON_GPIO		GPIOC

#define USB_UART_GPIO	GPIOD
#define USB_UART_TX		8
#define USB_UART_RX		9
/*****************************************************************************/
/*****************************************************************************/

#endif
