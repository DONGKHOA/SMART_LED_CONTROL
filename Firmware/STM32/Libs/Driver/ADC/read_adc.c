/*
 * read_adc.c
 *
 *  Created on: Apr 9, 2024
 *      Author: dongkhoa
 */
#include "read_adc.h"
#include "FreeRTOS.h"
#include "task.h"


int16_t read_adc(ADC_HandleTypeDef hadc1)
{
    HAL_ADC_Start(&hadc1);
    vTaskDelay(50);
    int16_t var = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);
    return var;
}