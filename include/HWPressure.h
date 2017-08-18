#ifndef _HWPressure__H__
#define _HWPressure__H__

#include "i2c.h"
#include <stdint.h>
#include <float.h>


#define PRESSURE_ADDRESS		0x78				
#define OUTPUTMAX			14746
#define OUTPUTMIN			1638
#define PRESSUREMAX			87.0228
#define PRESSUREMIN			0

void HPread(void);
double HPgetStatus(void);
double HPgetPressure(void);
double HPgetTemperature(void);

#endif
