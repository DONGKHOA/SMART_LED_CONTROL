/*
 * calibrate_adc.c
 *
 *  Created on: Apr 9, 2024
 *      Author: dongkhoa
 */
#include "calibrate_adc.h"

int calibrate_adc(int var)
 {
	int y = (float)var* 0.9769 + 157.13;
    	return y;
 }
