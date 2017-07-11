#ifndef _CONFIG__H
#define _CONFIG__H

#include <stdio.h>
#include <stdint.h>
#include "nuc144.h" /* TODO: make this conditional */
#include "proc/defs.h" /* TODO: should this main processor header file have this generic name or should it be generated? */

/******************************************************************************/
/*                           Utility Definitions                              */
/******************************************************************************/
#define APB1_F	SystemCoreClock >> APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE1) >> RCC_CFGR_PPRE1_Pos]
#define APB2_F	SystemCoreClock >> APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE2) >> RCC_CFGR_PPRE2_Pos]
#define HCLK	SystemCoreClock
#define NEWLINE_GUARD   (curr == '\n' && prev != '\r') || (curr == '\r' && prev != '\n')

typedef struct {
	uint32_t produce_count, consume_count;
	uint16_t buffer_SIZE, message_available;
	char *array;
} PC_Buffer;
extern PC_Buffer usart3_tx, usart3_rx;
extern int _write(int fd, const void *buf, size_t count);
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                                   Globals                                  */
/******************************************************************************/
#define DEBUG_UART	USB_UART
#define DEBUG_BAUD	115200
#define USB_UART	USART3
#define USB_RX		usart3_rx
#define USB_TX		usart3_tx
extern PC_Buffer	USB_RX;
extern uint32_t SystemCoreClock;
extern volatile unsigned int ticks;
extern void SystemCoreClockUpdate(void);
extern const uint8_t AHBPrescTable[16];
extern const uint8_t APBPrescTable[8];
#define BOARD "STM32 Nucleo144 F767ZI"
#define PROCESSOR "STM32F767ZI"
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                                 Oscillator                                 */
/******************************************************************************/
#define HSE_VALUE	((uint32_t) 8000000) 
#define HSI_VALUE	((uint32_t) 16000000) 
#define LSE_VALUE	((uint32_t)	32768)
/** System Clock source            = PLL (HSE w/ HSE_BYP, 8 MHz)
  * SYSCLK(Hz)                     = 160000000 (160 MHz)
  * HCLK(Hz)                       = 160000000 (160 MHz)
  * AHB, APB1, APB2 Prescalars     = 1, 4, 2
  *
  * Constraints:
  * PLLN[8:0] - 50 <= PLLN <= 432 (f_VCO must be between 100-432 MHz)
  * PLLM[5:0] - 2 <= PLLM <= 63 (f_PLL_in must be 1-2 MHz)
  * PLLP[1:0] - do not exceed 180 MHz (0 = 2, 1 = 4, 2 = 6, 3 = 8)
  * PLLQ[3:0] - 2 <= PLLQ <= 15 goal is to create 48 MHz (USB, SDMMC RNG clock src)
  * PLLR[2:0] - 2 <= PLLR <= 7 DSI clock src 
  */
#define PLLN_VAL (((uint32_t) 160) << RCC_PLLCFGR_PLLN_Pos)	/* f_VCO = f_PLL_in * (PLLN / PLLM) */
#define PLLM_VAL (((uint32_t) 4) << RCC_PLLCFGR_PLLM_Pos)	/* 320 MHz = 8 MHz * (160 / 8)		*/
#define PLLP_VAL (((uint32_t) 0) << RCC_PLLCFGR_PLLP_Pos)	/* f_PLL_out = f_VCO / PLLP	(2)		*/
#define PLLQ_VAL (((uint32_t) 10) << RCC_PLLCFGR_PLLQ_Pos)	/* f_USB_SDMMC_RNG = f_VCO / PLLQ	*/
#define PLLR_VAL (((uint32_t) 5) << RCC_PLLCFGR_PLLR_Pos)	/* f_PLL_DSI_out = f_VCO / PLLR		*/
/******************************************************************************/
/******************************************************************************/


/************************* Miscellaneous Configuration ************************/
/* Uncomment if you need to relocate your vector Table in internal SRAM. */
/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET  0x00 /* Vector Table base offset, multiple of 0x200. */
/******************************************************************************/

#endif

