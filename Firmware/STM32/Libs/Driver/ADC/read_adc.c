/*
 * read_adc.c
 *
 *  Created on: Apr 9, 2024
 *      Author: dongkhoa
 */
#include "read_adc.h"
#include "FreeRTOS.h"
#include "task.h"


int16_t read_adc(ADC_HandleTypeDef hadc2)
{
    HAL_ADC_Start(&hadc2);
    vTaskDelay(50);
    int16_t var = HAL_ADC_GetValue(&hadc2);
    HAL_ADC_Stop(&hadc2);
    return var;
}
