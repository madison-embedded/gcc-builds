#include <stdio.h>
#include "config.h"
#include "rcc.h"

void print_post_info(void) {
	int i;
	printf("\r\n----------------------------------------------------------------------\r\n");
	printf("-        University of Wisconsin-Madison Embedded Engineering        -\r\n");
	printf("----------------------------------------------------------------------\r\n");
	printf("Board:\t\t%s\r\n", BOARD);
	printf("Processor:\t%s\r\n\n", PROCESSOR);

	printf("Oscillator Settings:\r\n");
	for (i = 0; i < NUM_CLK_SRCS; i++) {
		printf("%s:\t%s", clk_src_strings[i], rcc_getClockState((clk_src_t) i) ? "ON" : "OFF");
		if (i == HSE) printf("\t(%lu Hz), bypass %s", HSE_VALUE, rcc_getHSEBYP() ? "on" : "off");
		if (i == HSI) printf("\t(%lu Hz)", HSI_VALUE);
		if (i == LSE) printf("\t(%lu Hz), bypass %s", LSE_VALUE, rcc_getLSEBYP() ? "on" : "off");
		if (i == PLL) printf("\t(source: %s)", clk_src_strings[rcc_get_PLLClockSrc()]);
		printf("\r\n");
	}

	printf("\nFrequencies:\r\n");
	printf("SYSCLK:\t%lu Hz (source: %s)\r\n", SystemCoreClock, clk_src_strings[rcc_get_SysClockSrc()]);
	printf("APB1:\t%lu Hz\r\n", APB1_F);
	printf("APB2:\t%lu Hz\r\n", APB2_F);

	printf("\nUse 'help' for a list of commands.\r\n");
	printf("----------------------------------------------------------------------\r\n\r\n");
}

