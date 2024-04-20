/*
 * read_adc.h
 *
 *  Created on: Apr 9, 2024
 *      Author: Author: vuhuy
 */

#ifndef DRIVER_ADC_READ_ADC_H_
#define DRIVER_ADC_READ_ADC_H_

/***********************************************************************/

#include "stm32f1xx_hal.h"
#include "stdint.h"

int16_t read_adc(ADC_HandleTypeDef* hadc);
void temperature_sensor_init(ADC_HandleTypeDef* hadc, TIM_HandleTypeDef* htim);

/***********************************************************************/

#endif /* DRIVER_ADC_READ_ADC_H_ */
