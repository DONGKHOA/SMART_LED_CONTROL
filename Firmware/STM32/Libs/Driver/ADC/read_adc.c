#include "read_adc.h"

uint16_t ADC_RESULT[2];

int16_t read_adc (ADC_HandleTypeDef* hadc)
{
    HAL_ADC_Start(hadc);
    HAL_ADC_PollForConversion(hadc, 10);
    int16_t var = HAL_ADC_GetValue(hadc);
    HAL_ADC_Stop(hadc);
    return var;
}

void temperature_sensor_enable(ADC_HandleTypeDef* hadc)
{
	 HAL_ADC_Start_DMA(hadc, (uint32_t*)ADC_RESULT, 2);   // Start ADC Conversion
}
