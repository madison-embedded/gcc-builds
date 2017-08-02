#include "tim.h"

int TIM_PWM_Init(TIM_TypeDef *tim, uint8_t channel, uint32_t period, uint32_t duty)
{
	period = (uint32_t)(((SystemCoreClock/2) / 17570) - 1);
	/*enable TIM1 */
	RCC->APB2ENR |= 0x1;

	/*	edge aligned */
	tim->CR1 &= ~0x60;
	/*	count up*/
	tim->CR1 &= ~0x10;

	/*disable ARR*/
	tim->CR1 &= ~0x80;

	/*	set period */
	tim->ARR =period ;
	/*tim-> ARR = 0x8;*/

	tim->PSC = 0;

	tim->EGR |= 0x1;

	/*make pwm mode 1 */
	switch(channel){
		case 1:
	tim->CCMR1 |= 0x60;
	break;
		case 2:
	tim->CCMR1 |= 0x6000;
	break;
		case 3:
	tim->CCMR2 |= 0x60;
	break;
		case 4:
	tim->CCMR2 |= 0x6000;
	break;
		default:
	break;
	}
	
	/*clear and set active high*/
	tim->CCER &= ~(0xf<<(4*(channel-1)));

	tim->CR2 &= ~0x300;

	switch (channel){
		case 1:
			tim->CCR1 |= (uint32_t)(((uint32_t) duty * (period - 1)) / 100);
			break;
		case 2:
			tim->CCR2 |= (uint32_t)(((uint32_t) duty * (period - 1)) / 100);
			break;
		case 3:
			tim->CCR3 |= (uint32_t)(((uint32_t) duty * (period - 1)) / 100);
			break;
		case 4:
			tim->CCR4 |= (uint32_t)(((uint32_t) duty * (period - 1)) / 100);
			break;
		default:
			break;
	}
	/* output on pin */
	tim->BDTR |= 0x8000;

	/*	enable timer */
	tim->CR1 |= 0x1;
	/* enable channel */
	tim->CCER |= 0x1<<(4*(channel-1));

	return 1;
}

