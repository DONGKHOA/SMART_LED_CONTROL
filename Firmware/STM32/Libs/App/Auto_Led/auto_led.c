#include "auto_led.h"
#include "illuminance.h"

extern int16_t lux;

uint8_t autocontrol_mode()
{
	float volt = voltage_adc();
	float ev_before = illuminance_adc(volt);
  lux = adjust_Ev(ev_before);
	if (illuminance_signal(lux) == 1)
	{
		turnOnLight();
		return 1;
	}

	else
	{
		turnOffLight();
		return 0;
	}
}
