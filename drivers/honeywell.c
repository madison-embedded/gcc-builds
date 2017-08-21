#include "honeywell.h"
#include "config.h"
#include "hal/hal_conf.h"
#include "hal/stm32f7xx_hal_i2c.h"

extern I2C_HandleTypeDef hi2c;

int honeywell_readPressure(void) {
	int retval = 0;
	uint8_t data[2];
	HAL_StatusTypeDef stat;

	stat = HAL_I2C_Master_Receive(&hi2c, HONEYWELL_I2C_ADDR << 1, data, 2, 500);
	if (stat != HAL_OK)
		return -1;

	retval |= ((data[0] & 0x3f) << 8) | data[1];
	retval -= 1638;
	retval *= 15000;
	return retval / 13107;
}

int honeywell_readTemperature(void) {
	int retval = 0;
	uint8_t data[4];
	HAL_StatusTypeDef stat;

	stat = HAL_I2C_Master_Receive(&hi2c, HONEYWELL_I2C_ADDR << 1, data, 4, 500);
	if (stat != HAL_OK)
		return -1;

	retval |= (data[2] << 8) | data[3];
	retval = retval >> 5;
	retval *= 200;
	retval /= 2047;
	return retval - 50;
}

bool honeywell_isAlive(void) {
	return (HAL_I2C_IsDeviceReady(&hi2c, HONEYWELL_I2C_ADDR << 1, 1, 500) == HAL_OK) ? true : false;
}

