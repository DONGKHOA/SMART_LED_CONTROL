/*
 * calibrate_adc.c
 *
 *  Created on: Apr 9, 2024
 *      Author: Author: vuhuy
 */
#include "calibrate_adc.h"

int16_t calibrate_adc(int16_t var)
{
	int16_t y = (float)var * 0.9769 + 157.13;
	return y;
}
