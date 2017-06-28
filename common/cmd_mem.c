#include <stdio.h>
#include "cli.h"
#include <string.h>
#include <stdlib.h>

command_status do_md(int argc, char *argv[]) {
	//printf("md not implemented\r\n");

	//Todo: input checking

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

command_status do_mw(int argc, char *argv[]) {
	printf("mw not implemented\r\n");
	return USAGE;
}
COMMAND_ENTRY("mw", "mw <addr> <value> <count>", "Write data memory contents.", do_mw)



command_status do_memmap(int argc, char *argv[]) {
	printf("HEEEY\r\n");
	return USAGE;
}


COMMAND_ENTRY("memmap", "mw <addr> <value> <count>", "Memory map", do_memmap)
