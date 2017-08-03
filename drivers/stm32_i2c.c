

#include "i2c.h"


volatile uint8_t* BUFFER_I2C = NULL;

volatile bool READ1_RDY = false;
volatile bool READ2_RDY = false;
volatile bool READ3_RDY = false;
volatile bool READ4_RDY = false;
volatile bool I2C1_BUSY = false;
volatile bool I2C2_BUSY = false;
volatile bool I2C3_BUSY = false;
volatile bool I2C4_BUSY = false;

void I2C_init(uint32_t i2c_base)
{
	I2C_setClock(i2c_base, true); // Enable clock
	I2C_enable(false,i2c_base);   // Disable I2C 
 	/*Set timing t0 100kHz*/
	I2C_setPreScaler(0xB,i2c_base);
	I2C_setSCLDEL(0x4, i2c_base);
	I2C_setSDADEL(0x2, i2c_base);
	I2C_setSCLH(0xf, i2c_base);
	I2C_setSCLL(0x13, i2c_base);
	I2C_clockStretch_off(false, i2c_base);
	I2C_enable(true, i2c_base); 
	I2C_Master_Config_slaveADDR(i2c_base, false, 0x70, false, true);
	I2C_enable_NVIC(i2c_base);
}


bool I2C_setClock(uint32_t i2c_base, bool state )
{

	uint32_t i2c ;
    /* 1) The following enables a clock for a specific I2C */
	switch(i2c_base)
    	{

    	   case I2C1_BASE:                 // Enable clock
        	i2c = RCC_APB1ENR_I2C1EN;
           	break;

	    case I2C2_BASE:                 // Enable clock
            	i2c = RCC_APB1ENR_I2C2EN;
            	break;

	    case I2C3_BASE:                 // Enable clock
            	i2c = RCC_APB1ENR_I2C3EN;
            	break;

            case I2C4_BASE:
            	i2c = RCC_APB1ENR_I2C4EN;
            	break;
	    default:
            	return false;

	}
      	
	
 	RCC -> APB1ENR &= ~ i2c;
	while ( RCC -> APB1ENR & i2c ){}

	if( state) 
		RCC -> APB1ENR |= i2c ; 
 
	return true;
}

bool I2C_verifyBase(uint32_t i2c_base)
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
bool I2C_set_analog_filter( bool analog_on, uint32_t i2c_base )
{

	I2C_TypeDef * i2c = ( I2C_TypeDef * ) i2c_base;
	if(!I2C_verifyBase(i2c_base))
		return false; 

	if(analog_on)

		i2c -> CR1 &= ~I2C_CR1_ANFOFF; 		
	else 
		i2c -> CR1 |= I2C_CR1_ANFOFF;
	return true;
	
} 

bool I2C_set_digital_filter(uint8_t digital_filter, uint32_t i2c_base)
{
	
	I2C_TypeDef * i2c = ( I2C_TypeDef * ) i2c_base;
	if(!I2C_verifyBase(i2c_base) || (digital_filter > 15 ))
		return false;

	i2c -> CR1 &= ~I2C_CR1_DNF_Msk;
	i2c -> CR1 |= (digital_filter << I2C_CR1_DNF_Pos); 
	return true; 
} 

bool  I2C_setPreScaler(uint8_t preScaler, uint32_t i2c_base)
{
	I2C_TypeDef * i2c = ( I2C_TypeDef * ) i2c_base;
	
	if(!I2C_verifyBase(i2c_base) || ( preScaler > 15 ))
		return false;

	i2c -> TIMINGR &=  ~I2C_TIMINGR_PRESC_Msk ; 
	i2c -> TIMINGR |= preScaler << I2C_TIMINGR_PRESC_Pos; 
	return true; 
	
}

bool I2C_setSCLDEL(uint8_t scldel, uint32_t i2c_base) 
{
	
	I2C_TypeDef * i2c = ( I2C_TypeDef * ) i2c_base;
	
	if(!I2C_verifyBase(i2c_base) || ( scldel > 15 ))
		return false;
	i2c -> TIMINGR &= ~I2C_TIMINGR_SCLDEL_Msk;
	i2c -> TIMINGR |= scldel << I2C_TIMINGR_SCLDEL_Pos;
	return true;

}


bool I2C_setSDADEL(uint8_t sdadel, uint32_t i2c_base) 
{
	
	I2C_TypeDef * i2c = ( I2C_TypeDef * ) i2c_base;
	
	if(!I2C_verifyBase(i2c_base) || ( sdadel > 15 ))
		return false;
	
	i2c -> TIMINGR &= ~I2C_TIMINGR_SDADEL_Msk;
	i2c -> TIMINGR |= sdadel << I2C_TIMINGR_SDADEL_Pos;
	return true;
}

bool I2C_setSCLH(uint8_t sclh, uint32_t i2c_base) 
{
	
	I2C_TypeDef * i2c = ( I2C_TypeDef * ) i2c_base;
	
	if(!I2C_verifyBase(i2c_base) || ( sclh > 15 ))
		return false;
	
	i2c -> TIMINGR &= ~I2C_TIMINGR_SCLH_Msk;
	i2c -> TIMINGR |= sclh << I2C_TIMINGR_SCLH_Pos;
	return true;
}


bool I2C_setSCLL(uint8_t scll, uint32_t i2c_base) 
{
	
	I2C_TypeDef * i2c = ( I2C_TypeDef * ) i2c_base;
	
	if(!I2C_verifyBase(i2c_base) || ( scll > 15 ))
		return false;
	
	i2c -> TIMINGR &= ~I2C_TIMINGR_SCLL_Msk;
	i2c -> TIMINGR |= scll << I2C_TIMINGR_SCLL_Pos;
	return true;
}



bool I2C_clockStretch_off(bool clockStretch_off, uint32_t i2c_base)
{
	I2C_TypeDef * i2c = NULL;

	if(I2C_verifyBase(i2c_base))
	       i2c = (I2C_TypeDef *) i2c_base;
	else return false; // illegal argument

	if( !clockStretch_off )  
		i2c -> CR1 &= ~I2C_CR1_NOSTRETCH_Msk; // must be kept cleared in master mode
	else 
		i2c -> CR1 |= I2C_CR1_NOSTRETCH; 

	return true;
}

bool I2C_enable(bool enable, uint32_t i2c_base)
{
	I2C_TypeDef * i2c = NULL;

	if(I2C_verifyBase(i2c_base))
	       i2c = (I2C_TypeDef *) i2c_base;
	else return false; // illegal argument

	 
	i2c -> CR1 &= ~I2C_CR1_PE_Msk;
	while( (i2c -> CR1 & I2C_CR1_PE) ){} 
	
	if(enable)
		i2c -> CR1 |= I2C_CR1_PE;
    	
	return true; 
		
}	

bool I2C_disable_NVIC(uint32_t i2c_base)
{	
   if(! I2C_verifyBase(i2c_base))
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

bool I2C_enable_NVIC(uint32_t i2c_base)
{	
    if(! I2C_verifyBase(i2c_base))
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
    
bool I2C_set10BitMode(bool ten_bit_mode, uint32_t i2c_base)
{
	
	I2C_TypeDef * i2c = NULL;

	if(I2C_verifyBase(i2c_base))
	       i2c = (I2C_TypeDef *) i2c_base;
	else return false; // illegal argument
	while (i2c -> CR2 & I2C_CR2_START_Msk) {}; // START bit set; illegal to change 
	if(ten_bit_mode)
		i2c -> CR2 |= I2C_CR2_ADD10; 
	else	
		i2c -> CR2 &= I2C_CR2_ADD10_Msk;

	return true; 
}

bool I2C_setSlaveAddr(uint32_t slave_addr, uint32_t i2c_base) 
{
	
	I2C_TypeDef * i2c = NULL;

	if(I2C_verifyBase(i2c_base))
	       i2c = (I2C_TypeDef *) i2c_base;
	else return false; // illegal argument
	while (i2c -> CR2 & I2C_CR2_START_Msk) {}; // START bit set; illegal to change 
	
	if( i2c -> CR2 & I2C_CR2_ADD10 )
	{
		if ( slave_addr >= 1024) // slave address must be 10 bits long 
			return false; 
		i2c -> CR2 &= ~I2C_CR2_SADD_Msk;
		i2c -> CR2 |= slave_addr; 
	}
	else
	{
		i2c -> CR2 &= ~I2C_CR2_SADD_Msk;
		i2c -> CR2 |= slave_addr << 1; 
	}	
	return true;
} 

bool I2C_setHead10R(bool  head10R, uint32_t i2c_base) 
{
	
	I2C_TypeDef * i2c = NULL;

	if(I2C_verifyBase(i2c_base))
	       i2c = (I2C_TypeDef *) i2c_base;
	else return false; // illegal argument
	
	while (i2c -> CR2 & I2C_CR2_START) {}; // START bit set; illegal to change 
	if(head10R)
		i2c -> CR2 |= I2C_CR2_HEAD10R;
	else
		i2c -> CR2 &= ~I2C_CR2_HEAD10R_Msk;
		
	return true; 	
}	

bool I2C_setAutoend(bool enable, uint32_t i2c_base) 
{
	
	I2C_TypeDef * i2c = NULL;

	if(I2C_verifyBase(i2c_base))
	       i2c = (I2C_TypeDef *) i2c_base;
	else return false; // illegal argument
	
	while (i2c -> CR2 & I2C_CR2_START) {}; // START bit set; illegal to change 
	if(enable) 
		i2c -> CR2 |= I2C_CR2_AUTOEND; 
	else 
		i2c -> CR2 &= ~I2C_CR2_AUTOEND_Msk; 

	return true;
}

bool I2C_StopDetect_Interrupt_Enable(bool enable, uint32_t i2c_base )
{
	
	I2C_TypeDef * i2c = NULL;

	if(I2C_verifyBase(i2c_base))
	       i2c = (I2C_TypeDef *) i2c_base;
	else return false; // illegal argument

	if(enable)
		i2c -> CR1 |= I2C_CR1_STOPIE; 
 	else 
		i2c -> CR1 &= ~I2C_CR1_STOPIE_Msk; 
	return true; 	
}

bool I2C_Transmit_Interrupt_Enable(bool enable, uint32_t i2c_base )
{
	
	I2C_TypeDef * i2c = NULL;

	if(I2C_verifyBase(i2c_base))
	       i2c = (I2C_TypeDef *) i2c_base;
	else return false; // illegal argument

	if(enable)
		i2c -> CR1 |= I2C_CR1_TXIE; 
 	else 
		i2c -> CR1 &= ~I2C_CR1_TXIE_Msk; 
	return true; 	
}

bool I2C_Receive_Interrupt_Enable(bool enable, uint32_t i2c_base )
{
	
	I2C_TypeDef * i2c = NULL;

	if(I2C_verifyBase(i2c_base))
	       i2c = (I2C_TypeDef *) i2c_base;
	else return false; // illegal argument

	if(enable)
		i2c -> CR1 |= I2C_CR1_RXIE; 
 	else 
		i2c -> CR1 &= ~I2C_CR1_RXIE_Msk; 
	return true; 	
}

bool I2C_Master_Config_slaveADDR(uint32_t i2c_base, bool ten_bit_mode, uint32_t slave_addr, bool head10R, bool autoend){
	I2C_TypeDef * i2c = (I2C_TypeDef * ) i2c_base;
	bool no_error;
	no_error =  I2C_set10BitMode(ten_bit_mode, i2c_base)| 
		    I2C_setSlaveAddr(slave_addr, i2c_base) |
		    I2C_setHead10R(head10R,i2c_base)|
		    I2C_setAutoend(autoend, i2c_base);	

 	i2c -> CR1 |= I2C_CR1_STOPIE  ; // Stop detection interrupt enable 

   	i2c -> CR1 |= I2C_CR1_TXIE; 	// Transmit interrupt enable
	i2c -> CR1 |= I2C_CR1_RXIE;   // Receive interrupt enable

	return no_error;
}

/* Transmit data */
bool I2C_Master_Transmit_Byte(I2C_TypeDef* i2c)
{
	if(BUFFER_I2C != NULL)
	{
		i2c -> TXDR = *BUFFER_I2C;
		BUFFER_I2C ++; // point to next byte

		return true;
	}
	else return false; // Todo: do something when the buffer is illegal address
}

bool I2C_Master_Receive_Byte(I2C_TypeDef* i2c)
{
	if(BUFFER_I2C != NULL)
	{
	    *BUFFER_I2C = i2c -> RXDR;
		BUFFER_I2C ++; // point to next byte

		return true;
	}
	else return false; // Todo: do something when the buffer is illegal address
}

void I2C_BusyCheck(uint32_t i2c_base)
{	
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

bool I2C_transmit_receive_config(uint32_t i2c_base, uint32_t nBytes, uint8_t* data_buffer, bool read) 
{ 
	I2C_TypeDef * i2c = NULL;

	if(I2C_verifyBase(i2c_base))
	       i2c = (I2C_TypeDef *) i2c_base;
	else return false; // illegal argument

	I2C_BusyCheck(i2c_base);
	
	i2c -> CR2 &= ~I2C_CR2_NBYTES_Msk; 
	i2c -> CR2 |=  nBytes << I2C_CR2_NBYTES_Pos; // Set the number of bytes
	i2c -> CR2 &= ~I2C_CR2_RD_WRN_Msk;  	     // Set read write bit to 0 
	if(read)
    		i2c -> CR2 |= I2C_CR2_RD_WRN; 
	BUFFER_I2C = data_buffer;

	if(!read)
  		I2C_Master_Transmit_Byte(i2c); 		     // put data into transmit buffer
	i2c -> CR2 |= I2C_CR2_START; 		     // START Transmitting

	return true;
}

bool I2C_Master_Transmit_Data(uint32_t i2c_base, uint32_t nBytes, uint8_t* data_buffer)
{
	bool transmit = I2C_transmit_receive_config(i2c_base,nBytes, data_buffer, false ); 
	return transmit;
}


bool I2C_Master_Receive_Data(uint32_t i2c_base, uint32_t nBytes, uint8_t* data_buffer)
{
	bool receive =  I2C_transmit_receive_config(i2c_base,nBytes, data_buffer, true );

	return receive;
}

void I2C_Read_Data_Ready(uint32_t i2c_base)
{
	switch(i2c_base)
	{
		case I2C1_BASE: 
			while(!READ1_RDY){};
			READ1_RDY = false;
			break;		
		case I2C2_BASE: 
			while(!READ2_RDY){};
			READ2_RDY = false;
			break;
		case I2C3_BASE: 
			while(!READ3_RDY){};
			READ3_RDY = false;
			break;
		case I2C4_BASE: 
			while(!READ4_RDY){};
			READ4_RDY = false;
		default: break;
	}
	
}


void I2C1_EV_IRQHandler()
{
	uint32_t isr = I2C1 -> ISR;
 	
	/* Transmitt Interrupt */
	if(isr & I2C_ISR_TXIS)
	{
		I2C_Master_Transmit_Byte(I2C1); 
	}
	else if(isr & I2C_ISR_RXNE) 
	{
		I2C_Master_Receive_Byte(I2C1);
	}
	else if (isr & I2C_ISR_STOPF )
	{
		I2C1->ICR |= I2C_ICR_STOPCF; 
		I2C1_BUSY = false;
			
		if( I2C1 -> CR2 & I2C_CR2_RD_WRN) READ1_RDY = true;
	}
}
void I2C2_EV_IRQHandler()
{
	uint32_t isr = I2C2 -> ISR;
 	
	/* Transmitt Interrupt */
	if(isr & I2C_ISR_TXIS)
	{
		I2C_Master_Transmit_Byte(I2C2); 
	}
	else if(isr & I2C_ISR_RXNE) 
	{
		I2C_Master_Receive_Byte(I2C2);
	}
	else if (isr & I2C_ISR_STOPF )
	{
		I2C2->ICR |= I2C_ICR_STOPCF; 
		I2C2_BUSY = false;	
		if( I2C2 -> CR2 & I2C_CR2_RD_WRN){
		 READ2_RDY = true;
		 }
	}
}
void I2C3_EV_IRQHandler()
{
	uint32_t isr = I2C3 -> ISR;
 	
	/* Transmitt Interrupt */
	if(isr & I2C_ISR_TXIS)
	{
		I2C_Master_Transmit_Byte(I2C3); 
	}
	else if(isr & I2C_ISR_RXNE) 
	{
		I2C_Master_Receive_Byte(I2C3);
	}
	else if (isr & I2C_ISR_STOPF )
	{
		I2C3->ICR |= I2C_ICR_STOPCF; 
		I2C3_BUSY = false;
			
		if( I2C3 -> CR2 & I2C_CR2_RD_WRN) READ3_RDY = true;
	}
}
void I2C4_EV_IRQHandler()
{
	uint32_t isr = I2C4 -> ISR;
 	
	/* Transmitt Interrupt */
	if(isr & I2C_ISR_TXIS)
	{
		I2C_Master_Transmit_Byte(I2C4); 
	}
	else if(isr & I2C_ISR_RXNE) 
	{
		I2C_Master_Receive_Byte(I2C4);
	}
	else if (isr & I2C_ISR_STOPF )
	{
		I2C4->ICR |= I2C_ICR_STOPCF; 
		I2C4_BUSY = false;
			
		if( I2C4 -> CR2 & I2C_CR2_RD_WRN) READ4_RDY = true;
	}
}

