#include "gpio.h"

static int verify_gpio_mode(GPIO_MODE mode) {
	if ((uint32_t) mode > 3) return -1;
	return 0;
}

static int verify_gpio_speed(GPIO_SPEED speed) {
	if ((uint32_t) speed > 3) return -1;
	return 0;
}

static int verify_gpio_pullup(GPIO_PULLUP_STATE state) {
	if ((uint32_t) state > 2) return -1;
	return 0;
}

static int verify_gpio_port(GPIO_TypeDef* port) {
	if (IS_GPIO_ALL_INSTANCE(port)) return 0;
	return -1;
}

int gpio_setClock(GPIO_TypeDef* port, bool state) {
	uint8_t bit_num = 0;
	switch ((uint32_t) port) {
		case GPIOA_BASE: bit_num = 0; break;
		case GPIOB_BASE: bit_num = 1; break;
		case GPIOC_BASE: bit_num = 2; break;
		case GPIOD_BASE: bit_num = 3; break;
		case GPIOE_BASE: bit_num = 4; break;
		case GPIOF_BASE: bit_num = 5; break;
		case GPIOG_BASE: bit_num = 6; break;
		case GPIOH_BASE: bit_num = 7; break;
		case GPIOI_BASE: bit_num = 8; break;
		case GPIOJ_BASE: bit_num = 9; break;
		case GPIOK_BASE: bit_num =10; break;
		default: return -1;
	}
	
	RCC->AHB1ENR &= ~(0x1 << bit_num);
	while (RCC->AHB1ENR & (0x1 << bit_num)) {;}
		
	if (state)
		RCC->AHB1ENR |= 0x1 << bit_num;
	
	return 0;
}

int gpio_setMode(GPIO_TypeDef* port, uint8_t pin, GPIO_MODE mode) {
	
	if (verify_gpio_port(port) || verify_gpio_mode(mode)
		|| pin > 15) return -1;
	
	port->MODER &= ~(0x3 << (pin*2));	/* clear mode	*/
	port->MODER |= mode << (pin*2);		/* set mode		*/
	return 0;
}

int gpio_openDrainState(GPIO_TypeDef* port, uint8_t pin, bool state) {
	
	if (verify_gpio_port(port) || pin > 15) return -1;
	
	port->OTYPER &= ~(0x1 << pin);			/* clear output-type state	*/
	if (state) port->OTYPER |= 0x1 << pin;	/* set open drain			*/
	return 0;
}

int gpio_setSpeed(GPIO_TypeDef* port, uint8_t pin, GPIO_SPEED speed) {
	
	if (verify_gpio_port(port) || pin > 15 ||
		verify_gpio_speed(speed)) return -1;
	
	port->OSPEEDR &= ~(0x3 << (pin*2));
	port->OSPEEDR |= speed << (pin*2);
	return 0;
}

int gpio_setPullupState(GPIO_TypeDef* port, uint8_t pin, GPIO_PULLUP_STATE state) {
	
	if (verify_gpio_port(port) || pin > 15 ||
		verify_gpio_pullup(state)) return -1;
	
	port->PUPDR &= ~(0x3 << (pin*2));
	port->PUPDR |= state << (pin*2);
	return 0;
}

int gpio_readPin(GPIO_TypeDef* port, uint8_t pin) {
	
	if (verify_gpio_port(port) || pin > 15) return -1;
	return port->IDR & 0x1 << pin;
}

inline void gpio_setPin(GPIO_TypeDef* port, uint8_t pin) {
	port->BSRR |= 0x1 << pin;
}

inline void gpio_resetPin(GPIO_TypeDef* port, uint8_t pin) {
	port->BSRR |= 0x1 << (pin + 16);
}

int gpio_writePin(GPIO_TypeDef* port, uint8_t pin, bool state) {
	
	if (verify_gpio_port(port) || pin > 15) return -1;
	state ? gpio_setPin(port, pin) :
			gpio_resetPin(port, pin);
	return 0;
}

int gpio_setAlternateFunc(GPIO_TypeDef* port, uint8_t pin, uint8_t val) {
	
	if (verify_gpio_port(port) || pin > 15 ||
		val > 15) return -1;
	
	port->AFR[pin / 8] &= ~(0xF << ((pin % 8) * 4));
	port->AFR[pin / 8] |= val << ((pin % 8) * 4);
	return 0;
}
