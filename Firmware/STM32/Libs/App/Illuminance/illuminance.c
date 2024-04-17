#include "illuminance.h"
#include "calibrate_adc.h"

float volt;
float Ev;
int16_t var;
extern ADC_HandleTypeDef hadc2;


float voltage_adc()
{
	var = read_adc(&hadc2);
	int16_t var_after = calibrate_adc(var);
	volt = (((float)var_after*3.3)/4096);
 	volt = volt/6;
	return volt;
}

float illuminance_adc()
{
	float R = volt*10; // (kOhm)
	Ev = R - 4.6974;
	Ev = Ev/(-1.02*10e-4);
	return Ev;
}

float illuminance_signal()
{
	if (volt < 9)
		return 0;
	else return 1;
}
