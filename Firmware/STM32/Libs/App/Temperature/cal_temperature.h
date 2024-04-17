#ifndef INC_CAL_TEMPERATURE_H_
#define INC_CAL_TEMPERATURE_H_
/***********************************************************/

#include "stm32f1xx_hal.h"
#include "string.h"
#define AVG_SLOPE (4.3F)
#define V_AT_25C  (1.43F)
#define V_REF_INT (1.2F)

float calculate_temperature(void);

/***********************************************************/
#endif /* INC_CAL_TEMPERATURE_H_ */
