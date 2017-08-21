#ifndef _HONEYWELL__H
#define _HONEYWELL__H

#include <stdbool.h>

#define HONEYWELL_I2C_ADDR		0x38

int honeywell_readPressure(void);
int honeywell_readTemperature(void);
bool honeywell_isAlive(void);

#endif

