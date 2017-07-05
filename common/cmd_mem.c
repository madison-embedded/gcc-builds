#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "proc/defs.h" 
#include "cli.h"



bool md_input_handler(unsigned char* mem_ptr, int length)
{
	unsigned char* mem = (unsigned char*) mem_ptr;
	int i;

	for(i = 0; i < length; i++)
	{	
		mem = mem + i;
		
		if((int) mem >= 0x00004000 && (int) mem <= 0x000FFFFF) return false; 
		
		if((int) mem >= 0x00110000 && (int) mem <= 0x001FFFFF) return false; 

		if((int) mem >= 0x00300000 && (int) mem <= 0x07FFFFFF) return false; 

		if((int) mem >= 0x08200000 && (int) mem <= 0x1FFEFFFF) return false; 

		if((int) mem >= 0x1FFF0020 && (int) mem <= 0x1FFFFFFF) return false; 

		if((int) mem >= 0x20080000 && (int) mem <= 0x3FFFFFFF) return false; 

		if((int) mem >= 0x40008000 && (int) mem <= 0x4000FFFF) return false; 

		if((int) mem >= 0x40016C00 && (int) mem <= 0x4001FFFF) return false; 

		if((int) mem >= 0x40080000 && (int) mem <= 0x4FFFFFFF) return false; 

		if((int) mem >= 0x50060C00 && (int) mem <= 0x5FFFFFFF) return false; 

		if((int) mem >= 0xE0100000 && (int) mem <= 0xFFFFFFFF) return false; 
	}

	return true; 
}


command_status do_md(int argc, char *argv[]) {

	if(argc != 3) return USAGE;
	
	if(argv[1][0] != '0' || argv[1][1] != 'x') return USAGE;

	int i;
	char content;
	int temp;

	char* address = (char*) malloc( 9 * sizeof(char)); 
   
	for(i = 0; i < 8; i++)
		address[i] = argv[1][i+2];	
    address[8] = '\0';

	
	unsigned char* mem_ptr = (unsigned char *) strtol(address, NULL, 16);
    
	if(mem_ptr == 0 && (strcmp(address, "00000000") != 0)) return USAGE;

	int length = atoi(argv[2]);
	if(length == 0 && (strcmp(argv[2], "0") != 0)) return USAGE;

	//input handling 
	if(!md_input_handler(mem_ptr, length)) 
	{	
		printf("Illegal memory Access\r\n");
		return FAIL;
	}

	char* ascii_content = (char*) malloc(17*sizeof(char));
	ascii_content[16] = '\0';
      
	for(i=0; i<length; i++)
	{
		content = (char) mem_ptr[i];
	
		if(i % 16 == 0) //new line
		{
 		
			printf("%8p:", &mem_ptr[i]);
		};
		if(i % 4 == 0) //space
		{
			printf(" ");
		}
		
		if(content >= 16)
			printf("%x", content);
		else 
			printf("0%x", content);
		ascii_content[i % 16] = (content >= 0x20 && content <= 0x7e) ? content : '.' ;
	
		if(i%16 == 15) printf(" %s\r\n", ascii_content);
	}

	if(length % 16 != 0) 
	{	
		temp = 16 - length%16;
		
		for(i = 0; i < temp; i++)
			ascii_content[length%16 + i] = ' ';

		temp = 2*temp + temp / 4;
		for(i = 0; i < temp; i++ )
			printf(" ");

		printf(" %s\r\n", ascii_content);
	}

	return SUCCESS;
}

COMMAND_ENTRY("md", "md <addr> <count>", "View raw memory contents.", do_md)


command_status do_mw(int argc, char *argv[])
{
	printf("mw not implemented\r\n");
	return USAGE;
}
COMMAND_ENTRY("mw", "mw <addr> <value> <count>", "Write data memory contents.", do_mw)


command_status do_memmap(int argc, char*argv[]) {
	printf("Bus\t\tPhysical\t\tTarget\t\t Size \r\n\n");
 	/*Cortex-M7*/
	
	/* AHB1 */

	
	printf("AHB1\t\tUSB OTG HS\t\t%x \t %d bytes \r\n",USB_OTG_HS_PERIPH_BASE,  0x4007ffff - USB_OTG_HS_PERIPH_BASE);
	printf("AHB1\t\tDMA2D_BASE\t\t%x \t %d bytes \r\n",DMA2D_BASE,       0x4002BBFF - DMA2D_BASE);
	printf("AHB1\t\tEthernet MAC\t\t%x \t %d bytes \r\n",0x40029000, 0x400293FF - 0x40029000);
	printf("AHB1\t\tEthernet MAC\t\t%x \t %d bytes \r\n",0x40028C00, 0x40028FFF - 0x40028C00);
	printf("AHB1\t\tEthernet MAC\t\t%x \t %d bytes \r\n",0x40028800, 0x40028BFF - 0x40028800);
	printf("AHB1\t\tEthernet MAC\t\t%x \t %d bytes \r\n",0x40028400, 0x400287FF - 0x40028400);
	printf("AHB1\t\tEthernet MAC\t\t%x \t %d bytes \r\n",0x40028000, 0x400283FF - 0x40028000);
	printf("AHB1\t\tDMA2\t\t\t%x \t %d bytes \r\n",DMA2_BASE,   0x400267FF - DMA2_BASE);
	printf("AHB1\t\tDMA1\t\t\t%x \t %d bytes \r\n",DMA1_BASE,   0x400263FF - DMA1_BASE); 
	printf("AHB1\t\tBKPSRAM\t\t\t%x \t %d bytes \r\n",BKPSRAM_BASE,0x40024FFF - BKPSRAM_BASE);
	printf("AHB1\t\tFLASH Interface\t\t%x \t %d bytes \r\n",FLASH_R_BASE, 0x40023FFF - FLASH_R_BASE);
	 
	printf("AHB1\t\tRCC\t\t\t%x \t %d bytes \r\n",RCC_BASE	   ,0x40023BFF - RCC_BASE);
	printf("AHB1\t\tCRC\t\t\t%x \t %d bytes \r\n",CRC_BASE        ,0x400233FF - CRC_BASE);
	printf("AHB1\t\tGPIOK\t\t\t%x \t %d bytes \r\n",GPIOK_BASE      ,0x40022BFF  - GPIOK_BASE);
	printf("AHB1\t\tGPIOJ\t\t\t%x \t %d bytes \r\n",GPIOJ_BASE      ,0x400227FF  - GPIOJ_BASE);
	printf("AHB1\t\tGPIOI\t\t\t%x \t %d bytes \r\n",GPIOI_BASE      ,0x400223FF  - GPIOI_BASE);
	printf("AHB1\t\tGPIOH\t\t\t%x \t %d bytes \r\n",GPIOH_BASE      ,0x40021FFF  - GPIOH_BASE); 
	printf("AHB1\t\tGPIOG\t\t\t%x \t %d bytes \r\n",GPIOG_BASE      ,0x40021BFF  - GPIOG_BASE);
	printf("AHB1\t\tGPIOF\t\t\t%x \t %d bytes \r\n",GPIOF_BASE     ,0x400217FF  - GPIOF_BASE);
	printf("AHB1\t\tGPIOE\t\t\t%x \t %d bytes \r\n",GPIOE_BASE      ,0x400213FF  - GPIOE_BASE);
	printf("AHB1\t\tGPIOD\t\t\t%x \t %d bytes \r\n",GPIOD_BASE      ,0x40020FFF  - GPIOD_BASE);
	printf("AHB1\t\tGPIOC\t\t\t%x \t %d bytes \r\n",GPIOC_BASE      ,0x40020BFF  - GPIOC_BASE);
	printf("AHB1\t\tGPIOB\t\t\t%x \t %d bytes \r\n",GPIOB_BASE      ,0x400207FF  - GPIOB_BASE);
	printf("AHB1\t\tGPIOA\t\t\t%x \t %d bytes \r\n\n",GPIOA_BASE      ,0x400203FF  - GPIOA_BASE); 
	
	/* END */
	

	/*APB2*/	
	printf("APB2\t\tMDIOS\t\t\t%x \t %d bytes \r\n",MDIOS_BASE      ,0x40017BFF  - MDIOS_BASE); 
	printf("APB2\t\tDFSDM1\t\t\t%x \t %d bytes \r\n",DFSDM1_BASE     ,0x400177FF  - DFSDM1_BASE); 
 	printf("APB2\t\tDSI Host\t\t%x \t %d bytes \r\n",0x40016C00     ,0x400173FF  - 0x40016C00); 
	printf("APB2\t\tLCD-HOST\t\t%x \t %d bytes \r\n",LTDC_BASE       ,0x40016BFF  - LTDC_BASE);
	printf("APB2\t\tSAI2\t\t\t%x \t %d bytes \r\n",SAI2_BASE       ,0x40015FFF  - SAI2_BASE);
	printf("APB2\t\tSAI1\t\t\t%x \t %d bytes \r\n",SAI1_BASE       ,0x40015BFF  - SAI1_BASE);
	printf("APB2\t\tSPI6\t\t\t%x \t %d bytes \r\n",SPI6_BASE       ,0x400157FF  - SPI6_BASE);
	printf("APB2\t\tSPI5\t\t\t%x \t %d bytes \r\n",SPI5_BASE       ,0x400153FF  - SPI5_BASE);
	printf("APB2\t\tTIM11\t\t\t%x \t %d bytes \r\n",TIM11_BASE      ,0x40014BFF  - TIM11_BASE);
	printf("APB2\t\tTIM10\t\t\t%x \t %d bytes \r\n",TIM10_BASE      ,0x400147FF  - TIM10_BASE);
	printf("APB2\t\tTIM9\t\t\t%x \t %d bytes \r\n",TIM9_BASE       ,0x400143FF  - TIM9_BASE);
	printf("APB2\t\tEXTI\t\t\t%x \t %d bytes \r\n",EXTI_BASE      ,0x40013FFF   - EXTI_BASE);
	printf("APB2\t\tSYSCFG\t\t\t%x \t %d bytes \r\n",SYSCFG_BASE      ,0x40013BFF   - SYSCFG_BASE);
	printf("APB2\t\tSPI4\t\t\t%x \t %d bytes \r\n",SPI4_BASE      ,0x400137FF   - SPI4_BASE);
	printf("APB2\t\tSPI1/I2S1\t\t%x \t %d bytes \r\n",SPI1_BASE      ,0x400133FF   - SPI1_BASE);
	printf("APB2\t\tSDMMC1\t\t\t%x \t %d bytes \r\n",SDMMC1_BASE      ,0x40012FFF   - SDMMC1_BASE);
	printf("APB2\t\tADC1-ADC2-ADC3\t\t%x \t %d bytes \r\n",ADC1_BASE      ,0x400123FF   - ADC1_BASE);
	printf("APB2\t\tSDMMC2\t\t\t%x \t %d bytes \r\n",SDMMC2_BASE      ,0x40011FFF   - SDMMC2_BASE);
	printf("APB2\t\tUSART6\t\t\t%x \t %d bytes \r\n",USART6_BASE      ,0x400117FF   - USART6_BASE);
	printf("APB2\t\tUSART1\t\t\t%x \t %d bytes \r\n",USART1_BASE      ,0x400113FF   - USART1_BASE);
	printf("APB2\t\tTIM8\t\t\t%x \t %d bytes \r\n",TIM8_BASE      ,0x400107FF   - TIM8_BASE);
	printf("APB2\t\tTIM1\t\t\t%x \t %d bytes \r\n\n",TIM1_BASE      ,0x400103FF   - TIM1_BASE);
	/*APB1*/
	
	printf("APB1\t\tUART8\t\t\t%x \t %d bytes \r\n",UART8_BASE      ,0x40007FFF   - UART8_BASE);
	printf("APB1\t\tUART7\t\t\t%x \t %d bytes \r\n",UART7_BASE      ,0x40007BFF   - UART7_BASE);
	printf("APB1\t\tDAC\t\t\t%x \t %d bytes \r\n",DAC_BASE      ,0x400077FF   - DAC_BASE);
	printf("APB1\t\tPWR\t\t\t%x \t %d bytes \r\n",PWR_BASE      ,0x400073FF   - PWR_BASE);
	printf("APB1\t\tHDMI-CEC\t\t%x \t %d bytes \r\n",0x40006C00      ,0x40006FFF   - 0x40006C00);
	printf("APB1\t\tCAN2\t\t\t%x \t %d bytes \r\n",CAN2_BASE      ,0x40006BFF   - CAN2_BASE);
	printf("APB1\t\tCAN1\t\t\t%x \t %d bytes \r\n",CAN1_BASE      ,0x400067FF   - CAN1_BASE);
	printf("APB1\t\tI2C4\t\t\t%x \t %d bytes \r\n",I2C4_BASE      ,0x400063FF   - I2C4_BASE);
	printf("APB1\t\tI2C3\t\t\t%x \t %d bytes \r\n",I2C3_BASE      ,0x40005FFF   - I2C3_BASE);
	printf("APB1\t\tI2C2\t\t\t%x \t %d bytes \r\n",I2C2_BASE      ,0x40005BFF   - I2C2_BASE);
	printf("APB1\t\tI2C1\t\t\t%x \t %d bytes \r\n",I2C1_BASE      ,0x400057FF   - I2C1_BASE);
	printf("APB1\t\tUART5\t\t\t%x \t %d bytes \r\n",UART5_BASE      ,0x400053FF   - UART5_BASE);
	printf("APB1\t\tUART4\t\t\t%x \t %d bytes \r\n",UART4_BASE      ,0x40004FFF   - UART4_BASE);
	printf("APB1\t\tUSART3\t\t\t%x \t %d bytes \r\n",USART3_BASE      ,0x40004BFF   - USART3_BASE);
	printf("APB1\t\tUSART2\t\t\t%x \t %d bytes \r\n",USART2_BASE      ,0x400047FF   - USART2_BASE);
	printf("APB1\t\tSPDIFRX\t\t\t%x \t %d bytes \r\n",SPDIFRX_BASE      ,0x400043FF   - SPDIFRX_BASE);
	printf("APB1\t\tSPI3/I2S3\t\t%x \t %d bytes \r\n",SPI3_BASE      ,0x40003FFF   - SPI3_BASE);
	printf("APB1\t\tSPI2/I2S2\t\t%x \t %d bytes \r\n",SPI2_BASE      ,0x40003BFF   - SPI2_BASE);
	printf("APB1\t\tCAN3\t\t\t%x \t %d bytes \r\n",CAN3_BASE      ,0x400037FF   - CAN3_BASE);
	printf("APB1\t\tIWDG\t\t\t%x \t %d bytes \r\n",IWDG_BASE      ,0x400033FF   - IWDG_BASE);
	printf("APB1\t\tWWDG\t\t\t%x \t %d bytes \r\n",WWDG_BASE      ,0x40002FFF   - WWDG_BASE);
	printf("APB1\t\tRTC & BKP rgstrs\t%x \t %d bytes \r\n",0x40002800      ,0x40002BFF   - 0X40002800);
	printf("APB1\t\tLPTIM1\t\t\t%x \t %d bytes \r\n",LPTIM1_BASE      ,0x400027FF   - LPTIM1_BASE);
	printf("APB1\t\tTIM14\t\t\t%x \t %d bytes \r\n",TIM14_BASE      ,0x400023FF   - TIM14_BASE);
	printf("APB1\t\tTIM13\t\t\t%x \t %d bytes \r\n",TIM13_BASE      ,0x40001FFF   - TIM13_BASE);
	printf("APB1\t\tTIM12\t\t\t%x \t %d bytes \r\n",TIM12_BASE      ,0x40001BFF   - TIM12_BASE);
	printf("APB1\t\tTIM7\t\t\t%x \t %d bytes \r\n",TIM7_BASE      ,0x400017FF   - TIM7_BASE);
	printf("APB1\t\tTIM6\t\t\t%x \t %d bytes \r\n",TIM6_BASE      ,0x400013FF   - TIM6_BASE);
	printf("APB1\t\tTIM5\t\t\t%x \t %d bytes \r\n",TIM5_BASE      ,0x40000FFF   - TIM5_BASE);
	printf("APB1\t\tTIM4\t\t\t%x \t %d bytes \r\n",TIM4_BASE      ,0x40000BFF   - TIM4_BASE);
	printf("APB1\t\tTIM3\t\t\t%x \t %d bytes \r\n",TIM3_BASE      ,0x400007FF   - TIM3_BASE);
	printf("APB1\t\tTIM2\t\t\t%x \t %d bytes \r\n\n",TIM2_BASE      ,0x400003FF   - TIM2_BASE);
	return SUCCESS; 
}
 

COMMAND_ENTRY("memmap", "memmap", "Display where different physical hardware peripherals", do_memmap); 
















