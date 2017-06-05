#ifndef _CONFIG__H
#define _CONFIG__H

#include <stdint.h>

#ifndef HSE_VALUE 
#define HSE_VALUE	((uint32_t)8000000)
#endif
#ifndef HSI_VALUE
#define HSI_VALUE	((uint32_t)16000000) 
#endif

extern uint32_t SystemCoreClock;
extern const uint8_t AHBPrescTable[16];
extern const uint8_t APBPrescTable[8];

extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);

#endif

