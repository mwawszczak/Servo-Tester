#include "pos_input.h"

volatile static uint16_t adc_reading;

void position_input_init(void) {
	HAL_ADCEx_Calibration_Start(&hadc1);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&adc_reading, 1);
}

uint16_t position_input_get (void) {
	uint16_t pos = 2000 - (((uint32_t)adc_reading * 1000) / 4095);
	return pos;
}
