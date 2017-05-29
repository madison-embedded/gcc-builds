#include "main.h"

void SystemClock_Config(void);
static void MPU_Config(void);
static void CPU_CACHE_Enable(void);

int main(void) {

  /* Configure the MPU attributes as Write Through */
  //MPU_Config();

  /* Enable the CPU Cache */
  //CPU_CACHE_Enable();
  
  /* Configure the system clock to 216 MHz */
  //SystemClock_Config();
  
  while (1) {

  }
}

/**
  * System Clock Configuration
  * The system Clock is configured as follow : 
  *    System Clock source            = PLL (HSI)
  *    SYSCLK(Hz)                     = 216000000
  *    HCLK(Hz)                       = 216000000
  *    AHB Prescaler                  = 1
  *    APB1 Prescaler                 = 4
  *    APB2 Prescaler                 = 2
  *    HSI Frequency(Hz)              = 16000000
  *    PLL_M                          = 16
  *    PLL_N                          = 432
  *    PLL_P                          = 2
  *    PLL_Q                          = 9
  *    PLL_R                          = 7
  *    VDD(V)                         = 3.3
  *    Main regulator output voltage  = Scale1 mode
  *    Flash Latency(WS)              = 7
  */
void SystemClock_Config(void) {

	/* Set FLASH latency */ 
	//LL_FLASH_SetLatency(LL_FLASH_LATENCY_7);

	/* Enable PWR clock */
	//LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

	/* Activation OverDrive Mode */
	//LL_PWR_EnableOverDriveMode();
	//while(LL_PWR_IsActiveFlag_OD() != 1) {;}

	/* Activation OverDrive Switching */
	//LL_PWR_EnableOverDriveSwitching();
	//while(LL_PWR_IsActiveFlag_ODSW() != 1) {;}
	
	/* Main PLL configuration and activation */
	//LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_16, 432, LL_RCC_PLLP_DIV_2);
	//LL_RCC_PLL_Enable();
	//while(LL_RCC_PLL_IsReady() != 1) {;}
	
	/* Sysclk activation on the main PLL */
	//LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	//LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
	//while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) {;}
	
	/* Set APB1 & APB2 prescaler*/
	//LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
	//LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
	
	/* Set systick to 1ms */
	//SysTick_Config(216000000 / 1000);
	
	/* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
	//SystemCoreClock = 216000000; 
}

/**
  * Configure the MPU attributes as Write Through for SRAM1/2.
  * The Base Address is 0x20020000 since this memory interface is the AXI.
  * The Configured Region Size is 512KB because the internal SRAM1/2 
  * memory size is 384KB.
  */

static void MPU_Config(void) {

	/* Disable MPU */
	//LL_MPU_Disable();

	/* Configure the MPU attributes as WT for SRAM */
	/*LL_MPU_ConfigRegion(LL_MPU_REGION_NUMBER0, 0x00, 0x20020000UL, 
		 LL_MPU_REGION_SIZE_512KB | LL_MPU_REGION_FULL_ACCESS | LL_MPU_ACCESS_NOT_BUFFERABLE |
		 LL_MPU_ACCESS_CACHEABLE | LL_MPU_ACCESS_SHAREABLE | LL_MPU_TEX_LEVEL0 |
		 LL_MPU_INSTRUCTION_ACCESS_ENABLE);*/

	/* Enable MPU (any access not covered by any enabled region will cause a fault) */
	//LL_MPU_Enable(LL_MPU_CTRL_PRIVILEGED_DEFAULT);
}

/* CPU L1-Cache enable. */
static void CPU_CACHE_Enable(void) {
	SCB_EnableICache();	/* Enable I-Cache */
	SCB_EnableDCache();	/* Enable D-Cache */
}

#ifdef  USE_FULL_ASSERT
/**
  * Reports the name of the source file and the source line number
  * where the assert_param error has occurred.
  * file: pointer to the source file name
  * line: assert_param error line source number
  */
void assert_failed(uint8_t *file, uint32_t line) {
	/* User can add his own implementation to report the file name and line number,
	   ex: printf("Wrong parameters value: file %s on line %d", file, line) */

	/* Infinite loop */
	while (1) {

	}
}
#endif

