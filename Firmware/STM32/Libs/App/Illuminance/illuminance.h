#ifndef APP_ILLUMINANCE_ILLUMINANCE_H_
#define APP_ILLUMINANCE_ILLUMINANCE_H_

/***********************************************************/

#include "stdint.h"
#include "calibrate_adc.h"
#include "event.h"
#include "stdio.h"
#include "math.h"
#include "queue.h"


float voltage_adc();
float illuminance_adc();
int illuminance_signal();
int16_t adjust_Ev();
void turnOnLight();
void turnOffLight();
void autocontrol_mode();

/***********************************************************/

#endif /* APP_ILLUMINANCE_ILLUMINANCE_H_ */
