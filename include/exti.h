#ifndef _exti__H__
#define _exti__H__

int exti_config(uint32_t pin, char port, bool rtsr, bool ftsr, bool ie);
void getNumInter(int pin);

#endif
