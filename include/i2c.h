#include <stdint.h>
#include <stdbool.h>
#include "proc/defs.h"

 
/******************************************************************************************************************/
/* I2C set Clock */
bool I2C_setClock(uint32_t i2c_base, bool state);

/******************************************************************************************************************/
/* I2C Noise Filter configure */
bool I2C_set_analog_filter(bool analog_on, uint32_t i2c_base);
bool I2C_set_digital_filter(uint8_t digital_filter, uint32_t i2c_base);

/******************************************************************************************************************/
/* I2C Timing configure */

/* I2C set PreScaler */
bool I2C_setPreScaler(uint8_t preScaler, uint32_t i2c_base);
/* I2C set SetupTime */
bool I2C_setSCLDEL(uint8_t scldel, uint32_t i2c_base);
/* I2C set HoldTime */
bool I2C_setSDADEL(uint8_t sdadel, uint32_t i2c_base);
/* I2C set HighPeriod */
bool I2C_setSCLH(uint8_t sclh, uint32_t i2c_base);
/* I2C set LowPeriod */
bool I2C_setSCLL(uint8_t scll, uint32_t i2c_base);

/******************************************************************************************************************/
/* I2C configure Clock Stetch */
bool I2C_clockStretch_off(bool clockStretch_off, uint32_t i2c_base);

/******************************************************************************************************************/
/* I2C peripheral enable */
bool I2C_enable(bool enable, uint32_t i2c_base);

/******************************************************************************************************************/
/* I2C NVIC configure */
bool I2C_enable_NVIC(uint32_t i2c_base);
bool I2C_disable_NVIC(uint32_t i2c_base);

/******************************************************************************************************************/
/* I2C Master Configure Slave Address */
bool I2C_Master_Config_slaveADDR(uint32_t i2c_base, bool ten_bit_mode, uint32_t slave_addr, bool head10R, bool autoend);
// Internal configurations
bool I2C_set10BitMode(bool ten_bit_mode, uint32_t i2c_base);
bool I2C_setSlaveAddr(uint32_t slave_addr, uint32_t i2c_base);
bool I2C_setHead10R(bool head10R, uint32_t i2c_base);
bool I2C_setAutoend(bool enable, uint32_t i2c_base);
bool I2C_StopDetect_Interrupt_Enable(bool enable, uint32_t i2c_base);
bool I2C_Transmit_Interrupt_Enable(bool enable, uint32_t i2c_base);
bool I2C_Receive_Interrupt_Enable(bool enable, uint32_t i2c_base);

/******************************************************************************************************************/
/* Transmit data N Bytes */
bool I2C_Master_Transmit_Data(uint32_t i2c_base, uint32_t nBytes, uint8_t* data_buffer);

/* Receive data N Bytes */
bool I2C_Master_Receive_Data(uint32_t i2c_base, uint32_t nBytes, uint8_t* data_buffer);

/* Read Data Ready (Valid)

Note: This function checks if the data_buffer passed to I2C_Master_Receive_Data contains new valid data;
After returning from this function, data_buffer contains the valid received data
*/
void I2C_Read_Data_Ready(uint32_t i2c_base);
void I2C_init(uint32_t i2c_base); 

