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
		
		if((unsigned int) mem >= 0x00004000 && (unsigned int) mem <= 0x000FFFFF) return false; 
		
		if((unsigned int) mem >= 0x00110000 && (unsigned int) mem <= 0x001FFFFF) return false; 

		if((unsigned int) mem >= 0x00300000 && (unsigned int) mem <= 0x07FFFFFF) return false; 

		if((unsigned int) mem >= 0x08200000 && (unsigned int) mem <= 0x1FFEFFFF) return false; 

		if((unsigned int) mem >= 0x1FFF0020 && (unsigned int) mem <= 0x1FFFFFFF) return false; 

		if((unsigned int) mem >= 0x20080000 && (unsigned int) mem <= 0x3FFFFFFF) return false; 

		if((unsigned int) mem >= 0x40008000 && (unsigned int) mem <= 0x4000FFFF) return false; 

		if((unsigned int) mem >= 0x40016C00 && (unsigned int) mem <= 0x4001FFFF) return false; 

		if((unsigned int) mem >= 0x40080000 && (unsigned int) mem <= 0x4FFFFFFF) return false; 

		if((unsigned int) mem >= 0x50060C00 && (unsigned int) mem <= 0x5FFFFFFF) return false; 

		if((unsigned int) mem >= 0xE0100000 && (unsigned int) mem <= 0xFFFFFFFF) return false; 
	}

	return true; 
}


command_status do_md(int argc, char *argv[]) {

	if(argc != 3) return USAGE;
	
	if(argv[1][0] != '0' || argv[1][1] != 'x') return USAGE;

	int i, j;
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
		printf("Illegal Memory Access\r\n");
		return FAIL;
	}

	char* ascii_content = (char*) malloc(17*sizeof(char));
	ascii_content[16] = '\0';
      
	for(i=0; i<length; i += 4)
	{
		if(i % 16 == 0) //new line
			printf("%8p:", &mem_ptr[i]);
		
		printf(" ");
		
		for(j=3; j>=0; j--)
		{
			content = (char) mem_ptr[i+j];
			
			if(content >= 16)
				printf("%x", content);
			else 
				printf("0%x", content);
			
			ascii_content[(i+3-j) % 16] = (content >= 0x20 && content <= 0x7e) ? content : '.' ;
	
		}
		
		
		if(i%16 == 15) 
			printf(" %s\r\n", ascii_content);
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
	printf("Physical\t\tTarget\t\t Size \r\n");
 
	/* AHB1 */


	printf("USB OTG HS\t\t%x \t %d bytes \r\n",USB_OTG_HS_PERIPH_BASE,  0x4007ffff - USB_OTG_HS_PERIPH_BASE);
	printf("DMA2D_BASE\t\t%x \t %d bytes \r\n",DMA2D_BASE,       0x4002BBFF - DMA2D_BASE);
	printf("Ethernet MAC\t\t%x \t %d bytes \r\n",0x40029000, 0x400293FF - 0x40029000);
	printf("Ethernet MAC\t\t%x \t %d bytes \r\n",0x40028C00, 0x40028FFF - 0x40028C00);
	printf("Ethernet MAC\t\t%x \t %d bytes \r\n",0x40028800, 0x40028BFF - 0x40028800);
	printf("Ethernet MAC\t\t%x \t %d bytes \r\n",0x40028400, 0x400287FF - 0x40028400);
	printf("Ethernet MAC\t\t%x \t %d bytes \r\n",0x40028000, 0x400283FF - 0x40028000);
	printf("DMA2\t\t\t%x \t %d bytes \r\n",DMA2_BASE,   0x400267FF - DMA2_BASE);
	printf("DMA1\t\t\t%x \t %d bytes \r\n",DMA1_BASE,   0x400263FF - DMA1_BASE); 
	printf("BKPSRAM\t\t\t%x \t %d bytes \r\n",BKPSRAM_BASE,0x40024FFF - BKPSRAM_BASE);
	printf("FLASH Interface\t\t%x \t %d bytes \r\n",FLASH_R_BASE, 0x40023FFF - FLASH_R_BASE);
	 
	printf("RCC\t\t\t%x \t %d bytes \r\n",RCC_BASE	   ,0x40023BFF - RCC_BASE);
	printf("CRC\t\t\t%x \t %d bytes \r\n",CRC_BASE        ,0x400233FF - CRC_BASE);
	printf("GPIOK\t\t\t%x \t %d bytes \r\n",GPIOK_BASE      ,0x40022BFF  - GPIOK_BASE);
	printf("GPIOJ\t\t\t%x \t %d bytes \r\n",GPIOJ_BASE      ,0x400227FF  - GPIOJ_BASE);
	printf("GPIOI\t\t\t%x \t %d bytes \r\n",GPIOI_BASE      ,0x400223FF  - GPIOI_BASE);
	printf("GPIOH\t\t\t%x \t %d bytes \r\n",GPIOH_BASE      ,0x40021FFF  - GPIOH_BASE); 
	printf("GPIOG\t\t\t%x \t %d bytes \r\n",GPIOG_BASE      ,0x40021BFF  - GPIOG_BASE);
	printf("GPIOF\t\t\t%x \t %d bytes \r\n",GPIOF_BASE     ,0x400217FF  - GPIOF_BASE);
	printf("GPIOE\t\t\t%x \t %d bytes \r\n",GPIOE_BASE      ,0x400213FF  - GPIOE_BASE);
	printf("GPIOD\t\t\t%x \t %d bytes \r\n",GPIOD_BASE      ,0x40020FFF  - GPIOD_BASE);
	printf("GPIOC\t\t\t%x \t %d bytes \r\n",GPIOC_BASE      ,0x40020BFF  - GPIOC_BASE);
	printf("GPIOB\t\t\t%x \t %d bytes \r\n",GPIOB_BASE      ,0x400207FF  - GPIOB_BASE);
	printf("GPIOA\t\t\t%x \t %d bytes \r\n",GPIOA_BASE      ,0x400203FF  - GPIOA_BASE); 
	
	/* END */
	

	/*APB2*/	
	printf("MDIOS\t\t\t%x \t %d bytes \r\n",MDIOS_BASE      ,0x40017BFF  - MDIOS_BASE); 
	printf("DFSDM1\t\t\t%x \t %d bytes \r\n",DFSDM1_BASE     ,0x400177FF  - DFSDM1_BASE); 
 	printf("DSI Host\t\t%x \t %d bytes \r\n",0x40016C00     ,0x400173FF  - 0x40016C00); 
	printf("LCD-HOST\t\t%x \t %d bytes \r\n",LTDC_BASE       ,0x40016BFF  - LTDC_BASE);
	printf("SAI2\t\t\t%x \t %d bytes \r\n",SAI2_BASE       ,0x40015FFF  - SAI2_BASE);
	printf("SAI1\t\t\t%x \t %d bytes \r\n",SAI1_BASE       ,0x40015BFF  - SAI1_BASE);
	printf("SPI6\t\t\t%x \t %d bytes \r\n",SPI6_BASE       ,0x400157FF  - SPI6_BASE);
	printf("SPI5\t\t\t%x \t %d bytes \r\n",SPI5_BASE       ,0x400153FF  - SPI5_BASE);
	printf("TIM11\t\t\t%x \t %d bytes \r\n",TIM11_BASE      ,0x40014BFF  - TIM11_BASE);
	printf("TIM10\t\t\t%x \t %d bytes \r\n",TIM10_BASE      ,0x400147FF  - TIM10_BASE);
	printf("TIM9\t\t\t%x \t %d bytes \r\n",TIM9_BASE       ,0x400143FF  - TIM9_BASE);
	printf("EXTI\t\t\t%x \t %d bytes \r\n",EXTI_BASE      ,0x40013FFF   - EXTI_BASE);
	printf("SYSCFG\t\t\t%x \t %d bytes \r\n",SYSCFG_BASE      ,0x40013BFF   - SYSCFG_BASE);
	printf("SPI4\t\t\t%x \t %d bytes \r\n",SPI4_BASE      ,0x400137FF   - SPI4_BASE);
	printf("SPI1/I2S1\t\t%x \t %d bytes \r\n",SPI1_BASE      ,0x400133FF   - SPI1_BASE);
	printf("SDMMC1\t\t\t%x \t %d bytes \r\n",SDMMC1_BASE      ,0x40012FFF   - SDMMC1_BASE);
	printf("ADC1-ADC2-ADC3\t\t%x \t %d bytes \r\n",ADC1_BASE      ,0x400123FF   - ADC1_BASE);
	printf("SDMMC2\t\t\t%x \t %d bytes \r\n",SDMMC2_BASE      ,0x40011FFF   - SDMMC2_BASE);
	printf("USART6\t\t\t%x \t %d bytes \r\n",USART6_BASE      ,0x400117FF   - USART6_BASE);
	printf("USART1\t\t\t%x \t %d bytes \r\n",USART1_BASE      ,0x400113FF   - USART1_BASE);
	printf("TIM8\t\t\t%x \t %d bytes \r\n",TIM8_BASE      ,0x400107FF   - TIM8_BASE);
	printf("TIM1\t\t\t%x \t %d bytes \r\n",TIM1_BASE      ,0x400103FF   - TIM1_BASE);
	/*APB1*/
	
	printf("UART8\t\t\t%x \t %d bytes \r\n",UART8_BASE      ,0x40007FFF   - UART8_BASE);
	printf("UART7\t\t\t%x \t %d bytes \r\n",UART7_BASE      ,0x40007BFF   - UART7_BASE);
	printf("DAC\t\t\t%x \t %d bytes \r\n",DAC_BASE      ,0x400077FF   - DAC_BASE);
	printf("PWR\t\t\t%x \t %d bytes \r\n",PWR_BASE      ,0x400073FF   - PWR_BASE);
	printf("HDMI-CEC\t\t%x \t %d bytes \r\n",0x40006C00      ,0x40006FFF   - 0x40006C00);
	printf("CAN2\t\t\t%x \t %d bytes \r\n",CAN2_BASE      ,0x40006BFF   - CAN2_BASE);
	printf("CAN1\t\t\t%x \t %d bytes \r\n",CAN1_BASE      ,0x400067FF   - CAN1_BASE);
	printf("I2C4\t\t\t%x \t %d bytes \r\n",I2C4_BASE      ,0x400063FF   - I2C4_BASE);
	printf("I2C3\t\t\t%x \t %d bytes \r\n",I2C3_BASE      ,0x40005FFF   - I2C3_BASE);
	printf("I2C2\t\t\t%x \t %d bytes \r\n",I2C2_BASE      ,0x40005BFF   - I2C2_BASE);
	printf("I2C1\t\t\t%x \t %d bytes \r\n",I2C1_BASE      ,0x400057FF   - I2C1_BASE);
	printf("UART5\t\t\t%x \t %d bytes \r\n",UART5_BASE      ,0x400053FF   - UART5_BASE);
	printf("UART4\t\t\t%x \t %d bytes \r\n",UART4_BASE      ,0x40004FFF   - UART4_BASE);
	printf("USART3\t\t\t%x \t %d bytes \r\n",USART3_BASE      ,0x40004BFF   - USART3_BASE);
	printf("USART2\t\t\t%x \t %d bytes \r\n",USART2_BASE      ,0x400047FF   - USART2_BASE);
	printf("SPDIFRX\t\t\t%x \t %d bytes \r\n",SPDIFRX_BASE      ,0x400043FF   - SPDIFRX_BASE);
	printf("SPI3/I2S3\t\t%x \t %d bytes \r\n",SPI3_BASE      ,0x40003FFF   - SPI3_BASE);
	printf("SPI2/I2S2\t\t%x \t %d bytes \r\n",SPI2_BASE      ,0x40003BFF   - SPI2_BASE);
	printf("CAN3\t\t\t%x \t %d bytes \r\n",CAN3_BASE      ,0x400037FF   - CAN3_BASE);
	printf("IWDG\t\t\t%x \t %d bytes \r\n",IWDG_BASE      ,0x400033FF   - IWDG_BASE);
	printf("WWDG\t\t\t%x \t %d bytes \r\n",WWDG_BASE      ,0x40002FFF   - WWDG_BASE);
	printf("RTC & BKP rgstrs\t%x \t %d bytes \r\n",0x40002800      ,0x40002BFF   - 0X40002800);
	printf("LPTIM1\t\t\t%x \t %d bytes \r\n",LPTIM1_BASE      ,0x400027FF   - LPTIM1_BASE);
	printf("TIM14\t\t\t%x \t %d bytes \r\n",TIM14_BASE      ,0x400023FF   - TIM14_BASE);
	printf("TIM13\t\t\t%x \t %d bytes \r\n",TIM13_BASE      ,0x40001FFF   - TIM13_BASE);
	printf("TIM12\t\t\t%x \t %d bytes \r\n",TIM12_BASE      ,0x40001BFF   - TIM12_BASE);
	printf("TIM7\t\t\t%x \t %d bytes \r\n",TIM7_BASE      ,0x400017FF   - TIM7_BASE);
	printf("TIM6\t\t\t%x \t %d bytes \r\n",TIM6_BASE      ,0x400013FF   - TIM6_BASE);
	printf("TIM5\t\t\t%x \t %d bytes \r\n",TIM5_BASE      ,0x40000FFF   - TIM5_BASE);
	printf("TIM4\t\t\t%x \t %d bytes \r\n",TIM4_BASE      ,0x40000BFF   - TIM4_BASE);
	printf("TIM3\t\t\t%x \t %d bytes \r\n",TIM3_BASE      ,0x400007FF   - TIM3_BASE);
	printf("TIM2\t\t\t%x \t %d bytes \r\n",TIM2_BASE      ,0x400003FF   - TIM2_BASE);
	return SUCCESS; 
}
 

COMMAND_ENTRY("memmap", "memmap", "Display where different physical hardware peripherals", do_memmap); 
















