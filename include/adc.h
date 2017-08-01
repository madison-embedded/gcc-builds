#ifndef _ADC__H__
#define _ADC__H__

#include "proc/defs.h"
#include <stdint.h>
#include <stdbool.h>

#define ADC_TARGET_FREQ		36000000 /* TODO: print this out */
#define ADC_SMP_BITS		5 /* TODO: change */

#define FILL_ADC(_adc, _channel, _port, _pin) \
	 {\
		.adc = _adc,\
		.channel = _channel,\
		.port = _port,\
		.pin = _pin,\
	},


typedef struct adcInfo{
	ADC_TypeDef *adc;
	uint8_t channel;
	char port;
	uint8_t pin;
} ADC_INFO;

extern const ADC_INFO ADC_LUT[];
extern const uint8_t NUM_ADC;
extern uint32_t adcFreq;

typedef struct adc_sample_time {

} adc_sample_time_t;

typedef struct adc_configuration {
	uint32_t cr[2];
	/* uint8_t sample_times[18]; */ /* don't need to mess with this for now */
	uint8_t sequence_channels[16];
	uint8_t num_conversions;
} adc_configuration_t;

uint16_t analogRead(ADC_TypeDef *adc, uint8_t channel);
bool adc_init(ADC_TypeDef *adc);

#endif

