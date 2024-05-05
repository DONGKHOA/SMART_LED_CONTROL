#include "calibrate_adc.h"


int16_t calibrate_adc(int16_t var)
{
	int16_t var_after = (float)var * 0.9769 + 157.13;
	return var_after;
}
