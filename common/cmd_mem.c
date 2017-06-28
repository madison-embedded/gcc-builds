#include <stdio.h>
#include "cli.h"
#include <string.h>


command_status do_md(int argc, char *argv[]) {
	//printf("md not implemented\r\n");

	//Todo: input checking

	unsigned char* mem_ptr = (unsigned char*) atoi(argv[1]);
	int length = atoi(argv[2]);
	char* ascii_content = (char*) malloc(16*sizeof(char));
	char content;
	int i; //loop count

	for(i=0; i<length; i++)
	{
		content = (char) mem_ptr[i];
 
		if(i % 16 == 0) //new line
		{
			printf(" %s\n", ascii_content);
			printf("%x:", &mem_ptr[i]);
		}
		if(i % 4 == 0) //space
		{
			printf(" ");
		}
		printf("%x", content);
		ascii_content[i % 16] = (content >= 0x20 && content <= 0x7e) ? content : '.' ;
	}
	
	return USAGE;
}
COMMAND_ENTRY("md", "md <addr> <count>", "View raw memory contents.", do_md)

command_status do_mw(int argc, char *argv[]) {
	printf("mw not implemented\r\n");
	return USAGE;
}
COMMAND_ENTRY("mw", "mw <addr> <value> <count>", "Write data memory contents.", do_mw)

