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

#endif
