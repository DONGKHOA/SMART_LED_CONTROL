#include "illuminance.h"
#include "calibrate_adc.h"
#include "event.h"

float volt;
float Ev;
int16_t var;
extern ADC_HandleTypeDef hadc2;
extern uint8_t autocontrol;


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

int illuminance_signal()
{
	if (volt < 9)
		return 0; //Turn the light off
	else return 1; //Turn the light on
}

void turnOnLight()
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
}

void turnOffLight()
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
}

void autocontrol_mode()
{
	if (autocontrol)
	{
		if (illuminance_signal() )
		turnOnLight();
		else turnOffLight();
	}
	else turnOffLight();
}
