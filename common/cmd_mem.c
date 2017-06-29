#include <stdio.h>
#include "proc/defs.h" 
#include "cli.h"
#include <string.h>
#include <stdlib.h>






command_status do_md(int argc, char *argv[]) {
	//Todo: input handling

	int i;
	char* address = (char*) malloc( 9 * sizeof(char)); 
   
	for(i = 0; i < 8; i++)
		address[i] = argv[1][i+2];
	
    address[8] = '\0';

	unsigned char* mem_ptr = (unsigned char *) strtol(address, NULL, 16);
    
	int length = atoi(argv[2]);
	char* ascii_content = (char*) malloc(17*sizeof(char));
	ascii_content[16] = '\0';
	char content;
	int temp;
      
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

	return USAGE;
}

COMMAND_ENTRY("md", "md <addr> <count>", "View raw memory contents.", do_md)

command_status do_mw(int argc, char *argv[])
{
	printf("mw not implemented\r\n");
	return USAGE;
}

COMMAND_ENTRY("mw", "mw <addr> <value> <count>", "Write data memory contents.", do_mw)


command_status do_memmap(int argc, char*argv[]) {
	printf("Physical     		 \t Target \t Size \r\n");
 
	/* AHB1 Start */

	printf("USB OTG HS   		 \t %x \t %d bytes \r\n",USB_OTG_HS,  0x4007ffff - (int)USB_OTG_HS);
	printf("DMA2D        		 \t %x \t %d bytes \r\n",DMA2D_BASE,       0x4002BBFF - DMA2D_BASE);
	//printf("Ethernet MAC \t %x \t %d bytes \n", 
	printf("DMA2	     		 \t %x \t %d bytes \r\n",DMA2_BASE,   0x400293FF - DMA2_BASE);
	printf("DMA1	     		 \t %x \t %d bytes \r\n",DMA1_BASE,   0x400263FF - DMA1_BASE); 
	printf("BKPSRAM	     		 \t %x \t %d bytes \r\n",BKPSRAM_BASE,0x40024FFF - BKPSRAM_BASE);
	printf("Flash interface register \t %x \t %d bytes \r\n",FLASH_R_BASE,0x40023FFF - FLASH_R_BASE); 
	printf("RCC			 \t %x \t %d bytes \r\n",RCC_BASE	   ,0x40023BFF - RCC_BASE);
	printf("CRC                      \t %x \t %d bytes \r\n",CRC_BASE        ,0x400233FF - CRC_BASE);
	printf("GPIOK                    \t %x \t %d bytes \r\n",GPIOK_BASE      ,0x40022BFF  - GPIOK_BASE);
	printf("GPIOJ                    \t %x \t %d bytes \r\n",GPIOJ_BASE      ,0x400227FF  - GPIOJ_BASE);
	printf("GPIOI                    \t %x \t %d bytes \r\n",GPIOI_BASE      ,0x400223FF  - GPIOI_BASE);
	printf("GPIOH                    \t %x \t %d bytes \r\n",GPIOH_BASE      ,0x40021FFF  - GPIOH_BASE); 
	printf("GPIOG                    \t %x \t %d bytes \r\n",GPIOG_BASE      ,0x40021BFF  - GPIOG_BASE);
	printf("GPIOF                    \t %x \t %d bytes \r\n",GPIOF_BASE     ,0x400217FF  - GPIOF_BASE);
	printf("GPIOE                    \t %x \t %d bytes \r\n",GPIOE_BASE      ,0x400213FF  - GPIOE_BASE);
	printf("GPIOD                    \t %x \t %d bytes \r\n",GPIOD_BASE      ,0x40020FFF  - GPIOD_BASE);
	printf("GPIOC                    \t %x \t %d bytes \r\n",GPIOC_BASE      ,0x40020BFF  - GPIOC_BASE);
	printf("GPIOB                    \t %x \t %d bytes \r\n",GPIOB_BASE      ,0x400207FF  - GPIOB_BASE);
	printf("GPIOA                    \t %x \t %d bytes \r\n",GPIOA_BASE      ,0x400203FF  - GPIOA_BASE); 
	
	/* END */
	
	
	printf("MDIOS                    \t %x \t %d bytes \r\n",MDIOS_BASE      ,0x40017BFF  - MDIOS_BASE); 
	printf("DFSDM1                   \t %x \t %d bytes \r\n",DFSDM1_BASE     ,0x400177FF  - DFSDM1_BASE); 
 //	printf("DSI Host                 \t %x \t %d bytes \n",DFSDM1_BASE,     ,0x400203FF  - DFSDM1_BASE); 
	printf("LCD-HOST                 \t %x \t %d bytes \r\n",LTDC_BASE       ,0x40016BFF  - LTDC_BASE);
	printf("SAI2                 	 \t %x \t %d bytes \r\n",SAI2_BASE       ,0x40015FFF  - SAI2_BASE);
	printf("SAI1                 	 \t %x \t %d bytes \r\n",SAI1_BASE       ,0x40015BFF  - SAI1_BASE);
	printf("SPI6                 	 \t %x \t %d bytes \r\n",SPI6_BASE       ,0x400157FF  - SPI6_BASE);
	printf("SPI5                 	 \t %x \t %d bytes \r\n",SPI5_BASE       ,0x400153FF  - SPI5_BASE);
	printf("TIM11                 	 \t %x \t %d bytes \r\n",TIM11_BASE      ,0x40014BFF  - TIM11_BASE);
	printf("TIM10                 	 \t %x \t %d bytes \r\n",TIM10_BASE      ,0x400147FF  - TIM10_BASE);
	printf("TIM9                 	 \t %x \t %d bytes \r\n",TIM9_BASE       ,0x400143FF  - TIM9_BASE);
	printf("EXTI                 	 \t %x \t %d bytes \r\n",EXTI_BASE      ,0x40013FFF   - EXTI_BASE);
	
}
 
COMMAND_ENTRY("memmap", "memmap", "Display memory map of physical hardware peripherals", do_memmap) 

















