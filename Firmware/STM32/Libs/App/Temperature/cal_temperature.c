#include "cal_temperature.h"
#include "calibrate_adc.h"
#include "stm32f1xx_hal.h"
#include "main.h"
#define AVG_SLOPE (4.3F)
#define V_AT_25C  (1.43F)
#define V_REF_INT (1.2F)

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;

float Temperature;

float calculate_temperature(int16_t * value_adc)
{
	uint16_t value_adc_low =  value_adc[0];

	value_adc_low = calibrate_adc(value_adc_low);
	Temperature = ((3.3*value_adc_low/4095 - V_AT_25C)/AVG_SLOPE)+25;

	return Temperature;
}
