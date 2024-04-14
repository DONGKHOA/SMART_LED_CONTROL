/*
 * cal_temperature.h
 *
 *  Created on: Apr 14, 2024
 *      Author: Admin
 */

#ifndef INC_CAL_TEMPERATURE_H_
#define INC_CAL_TEMPERATURE_H_
/***********************************************************/

#include "stm32f1xx_hal.h"
#include "string.h"
#define AVG_SLOPE (4.3F)
#define V_AT_25C  (1.43F)
#define V_REF_INT (1.2F)

void temperature_sensor_init(void);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
float calculate_temperature(void);

/***********************************************************/
#endif /* INC_CAL_TEMPERATURE_H_ */
