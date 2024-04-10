/*
 * illuminance.c
 *
 *  Created on: Apr 9, 2024
 *      Author: dongkhoa
 */

#include "illuminance.h"

float voltage_adc (int16_t y)
{
	float volt = (((float)y*3.3)/4096);
 	volt = volt/6;
	return volt;
}

float illuminance_adc (float volt)
{
	float R = volt*10; // (kOhm)
	float Ev = R - 4.6974;
	Ev = Ev/(-1.02*10e-4);
	return Ev;
}

float illuminance_signal (float volt)
{
	if (volt < 9)
		return 0;
	else return 1;
}
