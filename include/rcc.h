#ifndef _RCC__H__
#define _RCC__H__

#include <stdint.h>
#include <stdbool.h>
#include "proc/defs.h"

#define HSE_BYP	1	/* current nucleo144 config needs this */
#define LSE_BYP	0

typedef enum clk_src {
	HSI, HSE, PLL, PLLSAI, PLLI2S, LSI, LSE
} clk_src_t;

bool rcc_setClk(clk_src_t clk, bool state);
bool rcc_changeSysClockSrc(clk_src_t clk);
int rcc_getClockState(clk_src_t clk);
clk_src_t rcc_get_SysClockSrc(void);

#endif
