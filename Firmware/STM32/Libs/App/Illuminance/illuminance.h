/*
 * illuminance.h
 *
 *  Created on: Apr 9, 2024
 *      Author: dongkhoa
 */

#ifndef APP_ILLUMINANCE_ILLUMINANCE_H_
#define APP_ILLUMINANCE_ILLUMINANCE_H_

/***********************************************************/

#include "stdint.h"

float voltage_adc (int16_t y);
float illuminance_adc (float volt);
float illuminance_signal (float volt);

/***********************************************************/

#endif /* APP_ILLUMINANCE_ILLUMINANCE_H_ */
