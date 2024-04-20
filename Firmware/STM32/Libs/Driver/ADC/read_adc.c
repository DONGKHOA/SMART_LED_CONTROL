/*
 * read_adc.c
 *
 *  Created on: Apr 9, 2024
 *      Author: Author: vuhuy
 */
#include "read_adc.h"
#include "FreeRTOS.h"
#include "task.h"

uint16_t ADC_RESULT[2];

int16_t read_adc (ADC_HandleTypeDef* hadc)
{
    HAL_ADC_Start(hadc);
    vTaskDelay(50);
    int16_t var = HAL_ADC_GetValue(hadc);
    HAL_ADC_Stop(hadc);
    return var;
}
void temperature_sensor_init(ADC_HandleTypeDef* hadc, TIM_HandleTypeDef* htim)
{
	 HAL_TIM_Base_Start(htim); // Start Timer3 (Trigger Source For ADC1)
	 HAL_ADC_Start_DMA(hadc, (uint32_t*)ADC_RESULT, 2);   // Start ADC Conversion
}
