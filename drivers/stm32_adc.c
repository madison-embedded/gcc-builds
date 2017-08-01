#include "config.h"
#include "adc.h"

const ADC_INFO ADC_LUT[] = {
	FILL_ADC(ADC3, 3,  'A', 3)
	FILL_ADC(ADC1, 9,  'B', 1)
	FILL_ADC(ADC3, 10, 'C', 0)
	FILL_ADC(ADC3, 12, 'C', 2)
	FILL_ADC(ADC3, 13, 'C', 3)
	FILL_ADC(ADC3, 9,  'F', 3)
	FILL_ADC(ADC3, 14, 'F', 4)
	FILL_ADC(ADC3, 15, 'F', 5)
	FILL_ADC(ADC3, 8,  'F', 10)
	FILL_ADC(ADC3, 4,  'F', 6)
	FILL_ADC(ADC3, 5,  'F', 7)
	FILL_ADC(ADC3, 6,  'F', 8)
	FILL_ADC(ADC3, 7,  'F', 9)
	FILL_ADC(ADC1, 4,  'A', 4)
	FILL_ADC(ADC1, 5,  'A', 5)
	FILL_ADC(ADC1, 6,  'A', 6)
};

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
		while (divisor <= 8 && APB2_F / divisor > ADC_TARGET_FREQ)
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
	while (!(adc->SR & ADC_SR_STRT || ADC->CSR & 0x10)) {;}
	while (!(adc->SR & ADC_SR_EOC || ADC->CSR & 0x2)) {;}

	/* read data */
	retval = adc->DR;

	/* turn off ADC */
	//adc->CR2 &= ~ADC_CR2_ADON;

	/* return data */
	return retval;
}

bool adc_init(ADC_TypeDef *adc) {
	uint32_t curr_tick = (uint32_t) ticks;

	/* initialize if necessary */
	if (!adc_init_clk(adc))
		return false;

	adc->CR2 = 0x0; adc->CR1 = 0x0;
	adc->CR1 |= ADC_CR1_RES_0; /* 10-bit accuracy */
	adc->CR2 |= ADC_CR2_ADON;
	while (curr_tick == ticks) {;} /* stabilization? */

	return true;
}

