#ifndef APP_ILLUMINANCE_ILLUMINANCE_H_
#define APP_ILLUMINANCE_ILLUMINANCE_H_

/***********************************************************/

#include "stdint.h"

float voltage_adc();
float illuminance_adc();
int illuminance_signal();
void turnOnLight();
void turnOffLight();
void autocontrol_mode();

/***********************************************************/

#endif /* APP_ILLUMINANCE_ILLUMINANCE_H_ */
