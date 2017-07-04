#ifndef _ADC__H__
#define _ADC__H__

#include "proc/defs.h"
#include <stdint.h>
#include <stdbool.h>

#define ADC_TARGET_FREQ		36000000 /* TODO: print this out */
#define ADC_SMP_BITS		5 /* TODO: change */

extern uint32_t adcFreq;

typedef struct adc_sample_time {

} adc_sample_time_t;

typedef struct adc_configuration {
	uint32_t cr[2];
	/* uint8_t sample_times[18]; */ /* don't need to mess with this for now */
	uint8_t sequence_channels[16];
	uint8_t num_conversions;
} adc_configuration_t;

uint16_t analogRead(ADC_TypeDef *adc, GPIO_TypeDef *port, uint8_t channel);

#endif

