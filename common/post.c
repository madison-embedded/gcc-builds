#include <stdio.h>
#include "config.h"
#include "rcc.h"

void print_post_info(void) {
	int i;
	printf("\r\n----------------------------------------------------------------------\r\n");
	printf("-        University of Wisconsin-Madison Embedded Engineering        -\r\n");
	printf("----------------------------------------------------------------------\r\n");
	for (i = 0; i < NUM_CLK_SRCS; i++) {
		printf("%s:\t%s", clk_src_strings[i], rcc_getClockState((clk_src_t) i) ? "ON" : "OFF");
		if (i == HSE) printf("\t(%lu Hz)", HSE_VALUE);
		if (i == HSI) printf("\t(%lu Hz)", HSI_VALUE);
		if (i == LSE) printf("\t(%lu Hz)", LSE_VALUE);
		if (i == PLL) printf("\t(source: %s)", clk_src_strings[rcc_get_PLLClockSrc()]);
		printf("\r\n");
	}

	printf("SYSCLK source:\t%s\r\n", clk_src_strings[rcc_get_SysClockSrc()]);

	printf("SystemCoreClock: %u kHz\r\n", (unsigned int) SystemCoreClock / 1000);
	printf("Use 'help' for a list of commands.\r\n");
	printf("----------------------------------------------------------------------\r\n\r\n");
}

