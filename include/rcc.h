#ifndef _RCC__H__
#define _RCC__H__

#include <stdint.h>
#include <stdbool.h>
#include "proc/defs.h"

#define HSE_BYP	1	/* current nucleo144 config needs this */
#define LSE_BYP	0
extern const char *clk_src_strings[];

#define NUM_CLK_SRCS 7
typedef enum clk_src {
	HSI = 0, HSE = 1, PLL = 2, PLLSAI = 3, PLLI2S = 4, LSI = 5, LSE = 6
} clk_src_t;

/* Setting */
bool rcc_setClk(clk_src_t clk, bool state);
bool rcc_changeSysClockSrc(clk_src_t clk);
int rcc_getClockState(clk_src_t clk);

/* Getting */
clk_src_t rcc_get_SysClockSrc(void);
clk_src_t rcc_get_PLLClockSrc(void);
int rcc_getHSEBYP(void);
int rcc_getLSEBYP(void);

#endif
