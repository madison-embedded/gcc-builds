#include "config.h"
#include "adc.h"

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

	adc_init_sampling(adc);

	return true;
}

uint16_t analogRead(ADC_TypeDef *adc, GPIO_TypeDef *port, uint8_t channel) {
	uint16_t retval = 0;
	uint32_t curr_tick = (uint32_t) ticks;

	/* initialize if necessary */
	if (adcFreq == 0)
		if (!adc_init_clk(adc))
			return -1;

	adc->CR2 = 0x0; adc->CR1 = 0x0;
	adc->CR1 |= ADC_CR1_RES; /* 6-bit accuracy */
	adc->CR2 |= ADC_CR2_ADON;
	while (curr_tick == ticks) {;} /* stabilization? */

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

	/* return data */
	return retval;
}

/* for future use */
bool adc_init(ADC_TypeDef *adc, adc_configuration_t *config) {
	int i, bitsToShift;
	__IO uint32_t *curr_reg;

	/* setup clock divisor if necessary */
	if (adcFreq == 0)
		if (!adc_init_clk(adc))
			return false;

	/* Set Control Registers */
	adc->CR1 = config->cr[0];
	adc->CR2 = config->cr[1] & ~ADC_CR2_ADON; /* don't set ADON yet */
	
	/* Clear sequence registers, set number of sequences */
	adc->SQR1 = 0x0; adc->SQR2 = 0x0; adc->SQR3 = 0x0;
	adc->SQR1 |= (config->num_conversions & 0xf) << ADC_SQR1_L_Pos;

	/* Initialize sequence channel values */
	curr_reg = &adc->SQR3;
	bitsToShift = 0;
	for (i = 0; i < config->num_conversions; i++) {
		if (i == 6 || i == 12) bitsToShift = 0;
		if (i == 6) curr_reg = &adc->SQR2;
		else if (i == 12) curr_reg = &adc->SQR1;
		*curr_reg |= (config->sequence_channels[i] & 0x1f) << bitsToShift;
		bitsToShift += 5;
	}
	
	adc->CR2 |= ADC_CR2_ADON;
	return true;
}
