/*
 * illuminance.c
 *
 *  Created on: Apr 9, 2024
 *      Author: dongkhoa
 */

#include "illuminance.h"

float illuminance(float y)
{
	float volt = (((float)y * 3.3) / 4096);
	volt = volt / 6;
	float R = volt * 10; // (kOhm)
	float Ev = R - 4.6974;
	Ev = Ev / (-1.02 * 10e-4);
	return Ev;
}
