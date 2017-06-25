#include <stdio.h>
#include "config.h"
#include "rcc.h"

void print_post_info(void) {
	printf("\r----------------------------------------------------------------------\r\n");
	printf("HSI:\t%s\r\n", rcc_getClockState(HSI) ? "ON" : "OFF");
	printf("HSE:\t%s\r\n", rcc_getClockState(HSE) ? "ON" : "OFF");
	printf("LSI:\t%s\r\n", rcc_getClockState(LSI) ? "ON" : "OFF");
	printf("LSE:\t%s\r\n", rcc_getClockState(LSE) ? "ON" : "OFF");
	printf("PLL:\t%s\r\n", rcc_getClockState(PLL) ? "ON" : "OFF");
	printf("PLLSAI:\t%s\r\n", rcc_getClockState(PLLSAI) ? "ON" : "OFF");
	printf("PLLI2S:\t%s\r\n", rcc_getClockState(PLLI2S) ? "ON" : "OFF");

	printf("SYSCLK source: ");
	switch (rcc_get_SysClockSrc()) {
		case HSI: printf("HSI"); break;
		case HSE: printf("HSE"); break;
		case PLL: printf("PLL"); break;
		default: printf("UNKNOWN");
	}
	printf("\r\n");

	printf("SystemCoreClock: %u kHz\r\n", (unsigned int) SystemCoreClock / 1000);
	printf("Use 'help' for a list of commands.\r\n");
	printf("----------------------------------------------------------------------\r\n");
}

