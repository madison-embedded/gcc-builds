#include <stdio.h>
//#include "config.h"
#include "cli.h"
#include "timer.h"
#include "gpio.h"
#include "proc/defs.h" 
#include "../drivers/stm32_i2c.c"
extern void print_post_info(void);

void gpio_config(void)
{
	
	gpio_setClock(GPIOD, true);

	
	gpio_setAlternateFunc(GPIOD, 13, 4); 
	gpio_setAlternateFunc(GPIOD, 12, 4);

  	gpio_setMode(GPIOD, 13, ALT); 	    // Set pins to alt. function mode ( SDA)
	gpio_setMode(GPIOD, 12, ALT); 	    // SCL 
	

	gpio_openDrainState(GPIOD, 13, true); // set SDA as open drain 
	gpio_openDrainState(GPIOD, 12, true); // set SDA as open drain 

	gpio_setSpeed(GPIOD, 13, MEDIUM_SPEED); 
	gpio_setSpeed(GPIOD, 12, MEDIUM_SPEED);
 
	gpio_setPullupState(GPIOD, 13, NONE);
	gpio_setPullupState(GPIOD, 12, NONE);

	
}

void i2c_config(void)
{

	uint8_t  a[10];
    
	enable_I2C_clk(I2C4_BASE);
	enable_disable_I2C(I2C4_BASE, false);
	set_I2C_timing(I2C4_BASE, 0xB, 0x4, 0x2, 0xf,0x13);
	on_off_I2C_clockStretch(true, I2C4_BASE);
	enable_disable_I2C(I2C4_BASE, true);
	master_i2c_config(I2C4_BASE, 0, 0x70, 0); // master I2C init. 
	enable_i2c_NVIC(I2C4_BASE);
	
	a[0] = 0x21;
	master_i2c_trnsmt_rcv_data(I2C4_BASE, 1, a, 0);

	a[0] = 0xA0;
	master_i2c_trnsmt_rcv_data(I2C4_BASE, 1, a, 0);
	
	a[0] = 0xEF;
	master_i2c_trnsmt_rcv_data(I2C4_BASE, 1, a, 0);

	a[0] = 0x80;
	master_i2c_trnsmt_rcv_data(I2C4_BASE, 1, a, 0);
	
	a[0] = 0x00;
	a[1] = 0x3F;

	a[2] = 0x02;
	a[3] = 0x06;

	a[4] = 0x04;
	a[5] = 0x00;

	a[6] = 0x06;
	a[7] = 0x66;

	a[8] = 0x08;
	a[9] = 0x5B;

	master_i2c_trnsmt_rcv_data(I2C4_BASE, 10, a, 0);
	
	a[0] = 0x81; 
	master_i2c_trnsmt_rcv_data(I2C4_BASE, 1, a, 0);

	a[0] = 0x06; 
	master_i2c_trnsmt_rcv_data(I2C4_BASE, 1, a, 0);

	master_i2c_trnsmt_rcv_data(I2C4_BASE, 1, a, 1);

	read_received_data_I2C(I2C4_BASE);
		printf("Read: %x  \r\n", a[0] );
		printf("%x\r\n", a[1]);
		printf("%x\r\n", a[2]);
		printf("%x\r\n", a[3]);
		printf("%x\r\n", a[4]);
		
}



int main(void) {

 if (!board_init()){
		fault();
	}


//	print_post_info();
//	printPrompt();

	gpio_config();
	i2c_config();

	while(1) 
	{
	}
	
//		if (readButton()){
/*			setLED(1, true);*/
/*			setLED(2, true);*/
//		}
//		else {
/*			setLED(1, false);*/
/*			setLED(2, false);*/
//		}

//		check_input();

		/* Blink Red LED */
//		curr = ticks / 1000;
//		if (curr != prev) {
//			(curr % 2) ? 
//				setLED(0, true) : setLED(0, false);
//		}
//		prev = curr;
	//*/

}

