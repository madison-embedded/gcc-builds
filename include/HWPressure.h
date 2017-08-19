#ifndef _HWPressure__H__
#define _HWPressure__H__

#include "hal/stm32f7xx_hal.h"
#include "hal/stm32f7xx_hal_i2c.h"
#include <stdint.h>
#include <float.h>

#define PRESSURE_ADDRESS		0x78				
#define OUTPUTMAX			14746
#define OUTPUTMIN			1638
#define PRESSUREMAX			87.0228
#define PRESSUREMIN			0


extern I2C_HandleTypeDef hi2c;

void HPread(void);
double HPgetStatus(void);
double HPgetPressure(void);
double HPgetTemperature(void);

#endif
