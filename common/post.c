#include <stdio.h>
#include <time.h>
#include "config.h"
#include "rcc.h"
#include "hal/stm32f7xx_hal.h"

const unsigned int *UID = (const unsigned int *) UID_BASE;

void print_post_info(void) {
	int i;
	printf("\r\n----------------------------------------------------------------------\r\n");
	printf("-        University of Wisconsin-Madison Embedded Engineering        -\r\n");
	printf("----------------------------------------------------------------------\r\n");
	printf("Board:\t\t%s\r\n", BOARD);
	printf("Processor:\t%s (%08x-%08x-%08x)\r\n", PROCESSOR, UID[0], UID[1], UID[2]);
	printf("Flash Size:\t%uK\r\n\n", *((const unsigned int*) FLASHSIZE_BASE) & 0xffff);

	printf("Oscillator Settings:\r\n");
	for (i = 0; i < NUM_CLK_SRCS; i++) {
		printf("%s:\t%s", clk_src_strings[i], rcc_getClockState((clk_src_t) i) ? "ON" : "OFF");
		if (i == HSE) printf("\t(%lu kHz), bypass %s", HSE_VALUE / 1000, rcc_getHSEBYP() ? "on" : "off");
		if (i == HSI) printf("\t(%lu kHz)", HSI_VALUE / 1000);
		if (i == LSE) printf("\t(%lu Hz), bypass %s", LSE_VALUE, rcc_getLSEBYP() ? "on" : "off");
		if (i == PLL) printf("\t(source: %s)", clk_src_strings[rcc_get_PLLClockSrc()]);
		printf("\r\n");
	}

	printf("\nFrequencies:\r\n");
	printf("SYSCLK:\t%lu kHz (source: %s)\r\n", SystemCoreClock / 1000, clk_src_strings[rcc_get_SysClockSrc()]);
	printf("HCLK:\t%lu kHz\r\n", HAL_RCC_GetHCLKFreq() / 1000);
	printf("APB1:\t%lu kHz\r\n", (APB1_F) / 1000);
	printf("APB2:\t%lu kHz\r\n", (APB2_F) / 1000);

	char buff[40];
/*	strftime(buff, sizeof(_GIT_TIME), "%c", _GIT_TIME);*/
	printf("GitHub date: %s\r\n", buff);
	printf("Verson %d.%d compiled on: %s %s\r\n", _VERSION_MAJOR, _VERSION_MINOR, __DATE__, __TIME__);

	printf("\nUse 'help' for a list of commands.\r\n");
	printf("----------------------------------------------------------------------\r\n\r\n");
}
