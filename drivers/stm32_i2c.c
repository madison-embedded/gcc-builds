nclude "proc/defs.h"
#include <stdint.h>
#include <stdbool.h>

volatile uint8_t* BUFFER_I2C = NULL;

volatile bool READ1_RDY = false;
volatile bool READ2_RDY = false;
volatile bool READ3_RDY = false;
volatile bool READ4_RDY = false;
volatile bool I2C1_BUSY = false;
volatile bool I2C2_BUSY = false;
volatile bool I2C3_BUSY = false;
volatile bool I2C4_BUSY = false;

bool enable_I2C_clk(uint32_t i2c_base){

	int shift ;
    /* 1) The following enables a clock for a specific I2C */
	switch(i2c_base)
    {
    	case I2C1_BASE:                 // Enable clock
        	shift = 21;
            break;
		case I2C2_BASE:                 // Enable clock
            shift = 22;
            break;
		case I2C3_BASE:                 // Enable clock
            shift = 23;
            break;
        case I2C4_BASE:
            shift = 24;
            break;
		default:
            return false;
	}
       
 	RCC -> APB1ENR &= ~ ( 1 << shift );
	while ( RCC -> APB1ENR & 0x1 ){}
	RCC -> APB1ENR |= 1 << shift ; 
 
	return true;
}

bool verify_I2C_base(uint32_t i2c_base)
{
	switch(i2c_base)
    {
    	case I2C1_BASE: return true;

        case I2C2_BASE: return true;
               
        case I2C3_BASE: return true;        

		case I2C4_BASE: return true;

      	default: return false;         
    }
} 

bool config_I2C_filter(bool analog_on_off, uint8_t digital_filter_config, uint32_t i2c_base)
{
	I2C_TypeDef * i2c = NULL;

	if(verify_I2C_base(i2c_base))
	       i2c = (I2C_TypeDef *) i2c_base;
	else return false; // illegal argument

	/* Enables/Disables Analog filter: 
	   analog_on_off == true ==> ON
	   analog_on_off == false ==> OFF
	*/	
	if(analog_on_off)
		i2c -> CR1 &= ~(1<<12); 		
	else 
		i2c -> CR1 |= (1<<12);

	/*
	  Configure Digital Filter	
	*/
	if(digital_filter_config < 16)
	{
		i2c -> CR1 &= ~(0xF << 8);
		i2c -> CR1 |= (digital_filter_config << 8); 
	}
	else return false; // illegal argument 

	return true;
}

bool set_I2C_timing(uint32_t i2c_base , uint8_t preScaler, uint8_t setup_time, uint8_t hold_time, uint8_t sclh, uint8_t scll)
{	
	I2C_TypeDef * i2c = NULL;

	if(verify_I2C_base(i2c_base))
	       i2c = (I2C_TypeDef *) i2c_base;
	else return false; // illegal argument

	//Arguments should be less than 5 bits
    if(preScaler >= 16 || setup_time >= 16 || hold_time >= 16) 
                return false;

    i2c -> TIMINGR |= (preScaler << 28) | (setup_time << 20) | (hold_time << 16) 
					  | (sclh <<8) | (scll);
	return true;
}

bool on_off_I2C_clockStretch(bool on_off, uint32_t i2c_base)
{
	I2C_TypeDef * i2c = NULL;

	if(verify_I2C_base(i2c_base))
	       i2c = (I2C_TypeDef *) i2c_base;
	else return false; // illegal argument

	if(on_off)  i2c -> CR1 &= ~(1<<17); // must be kept cleared in master mode
			  
	else i2c -> CR1 |= (1<<17); 

	return true;
}

bool enable_disable_I2C(uint32_t i2c_base, bool en_dis)
{
	I2C_TypeDef * i2c = NULL;

	if(verify_I2C_base(i2c_base))
	       i2c = (I2C_TypeDef *) i2c_base;
	else return false; // illegal argument

	if(en_dis)
	{ 
		i2c -> CR1 &= ~(0x1);

		while( (i2c -> CR1 & 0x1) ){} 
		i2c -> CR1 |= 0x1;
    }
	else i2c -> CR1 &= ~(0x1);
	
	return true; 
		
}	

bool disable_i2c_NVIC(uint32_t i2c_base)
{	
	if(! verify_I2C_base(i2c_base))
		return false; // illegal argument

    /* 1) The following enables a clock for a specific I2C */
    switch(i2c_base)
    {
            case I2C1_BASE:                 // Enable clock
					NVIC_DisableIRQ(I2C1_EV_IRQn); // Enable Interrupt Handler
               		return true;

            case I2C2_BASE:                 // Enable clock     
					NVIC_DisableIRQ(I2C2_EV_IRQn); // Enable Interrupt Handler
	             	return true;

            case I2C3_BASE:                 // Enable clock	
					NVIC_DisableIRQ(I2C3_EV_IRQn); // Enable Interrupt Handler
		            return true; 

            case I2C4_BASE:	
					NVIC_DisableIRQ(I2C4_EV_IRQn); // Enable Interrupt Handler
	                return true;

            default:
                    return false;
     }

}    

bool enable_i2c_NVIC(uint32_t i2c_base)
{	
	if(! verify_I2C_base(i2c_base))
		return false; // illegal argument

    /* 1) The following enables a clock for a specific I2C */
    switch(i2c_base)
    {
            case I2C1_BASE:                 // Enable clock
					NVIC_EnableIRQ(I2C1_EV_IRQn); // Enable Interrupt Handler
               		return true;

            case I2C2_BASE:                 // Enable clock     
					NVIC_EnableIRQ(I2C2_EV_IRQn); // Enable Interrupt Handler
	             	return true;

            case I2C3_BASE:                 // Enable clock	
					NVIC_EnableIRQ(I2C3_EV_IRQn); // Enable Interrupt Handler
		            return true; 

            case I2C4_BASE:	
					NVIC_EnableIRQ(I2C4_EV_IRQn); // Enable Interrupt Handler
	                return true;

            default:
                    return false;
     }
}    
                                                  
bool master_i2c_config(uint32_t i2c_base, uint8_t ten_bit_mode, uint32_t slave_addr, uint8_t head10R) 
{
	I2C_TypeDef * i2c = NULL;

	if(verify_I2C_base(i2c_base))
	       i2c = (I2C_TypeDef *) i2c_base;
	else return false; // illegal argument

	/* Sets ten bit addressing mode or 7 bit mode 
	 and a slave address*/
	while (i2c -> CR2 & (1<<13)) {}; // START bit set; illegal to change 

	if(ten_bit_mode == 1)
	{
		i2c -> CR2 |= 1<<11;
		if ( slave_addr >= 1024) // slave address must be 10 bits long 
			return false; 
		else
		{	
			i2c -> CR2 &= 0xFFFFFC00;
			i2c -> CR2 |= slave_addr << 1; 
		}	 
	}
	else
	{
		i2c -> CR2 &= ~(1<<11);
		if(slave_addr >= 0x8F ) // argument should be 8 bits long
			return false;
		else
		{
			i2c -> CR2 &= 0xFFFFFC00;		
			i2c -> CR2 |= (slave_addr << 1);
		}
	}
	
	
	/*Sets transfer direction and head10R must be configure to indicate 
	 if the complete address sequence must be sent or only the header in 
	case of a direction change*/ 
	
	i2c -> CR2 &= ~(1 << 12);
	
	i2c -> CR2 |= (head10R << 12); 
  
		
	i2c -> CR2 |= (1<<25); // AutoEND
	
 	i2c -> CR1 |= 1 << 5 ; // Stop detection interrupt enable 
	
	i2c -> CR1 &= ~(0x6);

   	i2c -> CR1 |= 1 << 1; 	// Transmit interrupt enable
	i2c -> CR1 |= 1 << 2;   // Receive interrupt enable

	return true;
}

/* Transmit data */
bool transmit_byte_I2C_master(I2C_TypeDef* i2c)
{
	if(BUFFER_I2C != NULL)
	{
		i2c -> TXDR = *BUFFER_I2C;
		BUFFER_I2C ++; // point to next byte

		return true;
	}
	else return false; // Todo: do something when the buffer is illegal address
}

bool receive_byte_I2C_master(I2C_TypeDef* i2c)
{
	if(BUFFER_I2C != NULL)
	{
	    *BUFFER_I2C = i2c -> RXDR;
		BUFFER_I2C ++; // point to next byte

		return true;
	}
	else return false; // Todo: do something when the buffer is illegal address
}

void check_I2C_busy(uint32_t i2c_base)
{	
	I2C_TypeDef * i2c = NULL;

	if(verify_I2C_base(i2c_base))
	       i2c = (I2C_TypeDef *) i2c_base;
	else return false; // illegal argument

	switch(i2c_base)
	{
		case I2C1_BASE: 
			while(I2C1_BUSY){};
				I2C1_BUSY = true;
				break;
		case I2C2_BASE:
			while(I2C2_BUSY){};
				I2C2_BUSY = true;
				break;
		case I2C3_BASE:
			while(I2C3_BUSY){};
				I2C3_BUSY = true;
				break;
		case I2C4_BASE:
			while(I2C4_BUSY){};
				I2C4_BUSY = true;
				break;
		default: break;

	}

}
bool master_i2c_trnsmt_rcv_data(uint32_t i2c_base, uint32_t nmbr_of_bytes, uint8_t* data_buffer, uint8_t rd_wrt)
{
	I2C_TypeDef * i2c = NULL;

	if(verify_I2C_base(i2c_base))
	       i2c = (I2C_TypeDef *) i2c_base;
	else return false; // illegal argument

	check_I2C_busy(i2c_base);
	
	/*Sets the number of bytes to send*/
	i2c -> CR2 &= ~(0XFF << 16);
	i2c -> CR2 |=  nmbr_of_bytes << 16;
	i2c -> CR2 &= ~(1<<10);
    i2c -> CR2 |= rd_wrt << 10;
	
	BUFFER_I2C = data_buffer;
  
    if(!rd_wrt)
		transmit_byte_I2C_master(i2c); 		// put data into transmit buffer

	i2c -> CR2 |= (1<<13); 				// START

	return true;
}

bool read_received_data_I2C(uint32_t i2c_base)
{
	switch(i2c_base)
	{
		case I2C1_BASE: 
			while(!READ1_RDY){};
			READ1_RDY = false;
		
		case I2C2_BASE: 
			while(!READ2_RDY){};
			READ2_RDY = false;
		case I2C3_BASE: 
			while(!READ3_RDY){};
			READ3_RDY = false;
	
		case I2C4_BASE: 
			while(!READ4_RDY){};
			READ4_RDY = false;
		default: false;
	}
	return true;
}


void I2C1_EV_IRQHandler()
{
	uint32_t irs = I2C1 -> ISR;
	
	/* Transmitt Interrupt */
	if(irs & (1<<1)) transmit_byte_I2C_master(I2C1);
	else if(irs & (1<<2)) receive_byte_I2C_master(I2C1);
	else if (irs & (1 << 5) )
	{ 
		I2C1->ICR |= 1<<5; 
		I2C1_BUSY = false;

		if( I2C1 -> CR2 & (1<<10)) READ1_RDY = true;
	}
}

void I2C2_EV_IRQHandler()
{
	uint32_t irs = I2C2 -> ISR;
	
	/* Transmitt Interrupt */
	if(irs & (1<<1)) transmit_byte_I2C_master(I2C2); 
	else if(irs & (1<<2)) receive_byte_I2C_master(I2C2);
	else if (irs & (1 << 5) )
	{ 
		I2C2->ICR |= 1<<5; 
		I2C2_BUSY = false;

		if( I2C2 -> CR2 & (1<<10)) READ2_RDY = true;
	}
}
void I2C3_EV_IRQHandler()
{
	uint32_t irs = I2C3 -> ISR;
	
	/* Transmitt Interrupt */
	if(irs & (1<<1)) transmit_byte_I2C_master(I2C3); 
	else if(irs & (1<<2)) receive_byte_I2C_master(I2C3);
    else if (irs & (1 << 5) )
	{ 
		I2C3->ICR |= 1<<5; 
		I2C3_BUSY = false;
		
		if( I2C3 -> CR2 & (1<<10)) READ3_RDY = true;
	}
}
void I2C4_EV_IRQHandler()
{
	uint32_t irs = I2C4 -> ISR;
 	
	/* Transmitt Interrupt */
	if(irs & (1<<1))
	{
		transmit_byte_I2C_master(I2C4); 
	}
	else if(irs & (1<<2)) 
	{
		receive_byte_I2C_master(I2C4);
	}
	else if (irs & (1 << 5) )
	{
		I2C4->ICR |= 1<<5; 
		I2C4_BUSY = false;
			
		if( I2C4 -> CR2 & (1<<10)) READ4_RDY = true;
	}
}

