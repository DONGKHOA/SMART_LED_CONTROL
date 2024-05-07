#include "auto_led.h"
#include "illuminance.h"

uint8_t autocontrol_mode()
{
	if (illuminance_signal(adjust_Ev()) == 1)
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
