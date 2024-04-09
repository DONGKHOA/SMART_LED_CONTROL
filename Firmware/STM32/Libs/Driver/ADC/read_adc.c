/*
 * read_adc.c
 *
 *  Created on: Apr 9, 2024
 *      Author: dongkhoa
 */
#include "read_adc.h"
#include "stm32f1xx_hal.h"
#include "stdint.h"
#include "FreeRTOS.h"
#include "task.h"

extern ADC_HandleTypeDef hadc1;

int16_t read_adc (void)
{
    HAL_ADC_Start(&hadc1);
	vTaskDelay(50);
	int16_t var = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);
    return var;
}
