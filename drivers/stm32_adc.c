#include "config.h"
#include "adc.h"

const ADC_INFO ADC_LUT[] = {
	FILL_ADC(ADC3, 3,  GPIOA, 3)  /* A3:   Analog1 - Primary Battery Current */
	FILL_ADC(ADC1, 9,  GPIOB, 1)  /* B1:   Analog2 - Secondary Battery Voltage */
	FILL_ADC(ADC3, 10, GPIOC, 0)  /* C0:   Analog3 - Pressure 1 (CN5) */
	FILL_ADC(ADC3, 12, GPIOC, 2)  /* C2:   Analog4 - Pressure 2 (CN5) */
	FILL_ADC(ADC3, 13, GPIOC, 3)  /* C3:   Analog5 - Primary Battery Voltage */
	FILL_ADC(ADC3, 9,  GPIOF, 3)  /* F3:   Analog6 - Pressure 3 (CN5) */
	FILL_ADC(ADC3, 14, GPIOF, 4)  /* F4:   Analog7 - Pressure 4 (CN5) */
	FILL_ADC(ADC3, 15, GPIOF, 5)  /* F5:   Analog8 - Pressure 1 (CN6) */
	FILL_ADC(ADC3, 8,  GPIOF, 10) /* F10:  Analog9 - Pressure 2 (CN6) */
	FILL_ADC(ADC3, 4,  GPIOF, 6)  /* F6:  Analog10 - Accelerometer X */
	FILL_ADC(ADC3, 5,  GPIOF, 7)  /* F7:  Analog11 - Thermistor 3 */
	FILL_ADC(ADC3, 6,  GPIOF, 8)  /* F8:  Analog12 - Thermistor 4 */
	FILL_ADC(ADC3, 7,  GPIOF, 9)  /* F9:  Analog13 - Secondary Battery Current */
	FILL_ADC(ADC1, 4,  GPIOA, 4)  /* A4:  Analog14 - Thermistor 1 */
	FILL_ADC(ADC1, 5,  GPIOA, 5)  /* A5:  Analog15 - Thermistor 2 */
	//FILL_ADC(ADC1, 6,  GPIOA, 6)  /* A6:  Analog16 - */
};

#define AVERAGE_SIZE 100
int filter[2][16][AVERAGE_SIZE];
uint32_t count[2][16];
int average[2][16];

const uint8_t NUM_ADC = sizeof(ADC_LUT)/sizeof(ADC_INFO);

uint32_t adcFreq = 0;

static void adc_init_sampling(ADC_TypeDef *adc) {
	int i, bitsToShift;
	__IO uint32_t *curr_reg;

	/* Initialize Sampling Cycles */
	adc->SMPR1 = 0x0; adc->SMPR2 = 0x0;
	curr_reg = &adc->SMPR2;
	bitsToShift = 0;
	for (i = 0; i < 18; i++) {
		if (i == 10) {
			curr_reg = &adc->SMPR1;
			bitsToShift = 0;
		}
		*curr_reg |= ADC_SMP_BITS << bitsToShift;
		bitsToShift += 3;
	}
}

static bool adc_init_clk(ADC_TypeDef *adc) {
	uint8_t divisor = 2;
	uint32_t mask = 0;

	/*enable clock in RCC register */
	switch ((uint32_t) adc) {
		case ADC1_BASE: mask = RCC_APB2ENR_ADC1EN; break;
		case ADC2_BASE: mask = RCC_APB2ENR_ADC2EN; break;
		case ADC3_BASE: mask = RCC_APB2ENR_ADC3EN; break;
	}
	RCC->APB2ENR |= mask;

	if(adcFreq == 0){
		while (divisor <= 8 && ((APB2_F / divisor) > ADC_TARGET_FREQ))
			divisor += 2;
		if (divisor > 8) return false;
		adcFreq = APB2_F / divisor;
		ADC->CCR &= ~ADC_CCR_ADCPRE_Msk;
		switch (divisor) {
			case 4: ADC->CCR |= ADC_CCR_ADCPRE_0; break;
			case 6: ADC->CCR |= ADC_CCR_ADCPRE_1; break;
			case 8: ADC->CCR |= ADC_CCR_ADCPRE_1 | ADC_CCR_ADCPRE_0; break;
		}
	}

	adc_init_sampling(adc);

	return true;
}

uint16_t analogRead(ADC_TypeDef *adc, uint8_t channel) {
	uint16_t retval = 0;

	/* single conversion */
	adc->SQR1 = 0x0; adc->SQR3 = 0x0;

	/* set channel */
	adc->SQR3 |= channel & 0x1f;

	/* start conversion */
	adc->CR2 |= ADC_CR2_SWSTART;

	/* wait to start, wait to end */
	while (!(adc->SR & ADC_SR_STRT)) {;}
	while (!(adc->SR & ADC_SR_EOC)) {;}

	/* read data */
	retval = adc->DR;

	/* turn off ADC */
	//adc->CR2 &= ~ADC_CR2_ADON;

	if (adc == ADC3) {
		average[0][channel] -= filter[0][channel][count[0][channel] % AVERAGE_SIZE];
		average[0][channel] += retval;
		filter[0][channel][count[0][channel] % AVERAGE_SIZE] = retval;

		count[0][channel]++;

		/* return data */
		return average[0][channel] / AVERAGE_SIZE;
	}
	average[1][channel] -= filter[1][channel][count[1][channel] % AVERAGE_SIZE];
	average[1][channel] += retval;
	filter[1][channel][count[1][channel] % AVERAGE_SIZE] = retval;

	count[1][channel]++;

	/* return data */
	return average[1][channel] / AVERAGE_SIZE;
}


bool adc_init(ADC_TypeDef *adc) {
	uint32_t curr_tick = (uint32_t) ticks;
	uint8_t i, ii;

	/* initialize if necessary */
	if (!adc_init_clk(adc))
		return false;

	adc->CR2 = 0x0; adc->CR1 = 0x0;
	adc->CR1 |= ADC_CR1_RES_0; /* 10-bit accuracy */
	adc->CR2 |= ADC_CR2_ADON;
	while (curr_tick == ticks) {;} /* stabilization? */
	for (i = 0; i < 16; i++) {
		for(ii = 0; ii < AVERAGE_SIZE; ii++){
			analogRead(adc, i);
		}
	}

	return true;
}

