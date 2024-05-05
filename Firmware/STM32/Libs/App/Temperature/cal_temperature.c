#include "cal_temperature.h"
#include "calibrate_adc.h"
#include "stm32f1xx_hal.h"
#include "main.h"

#include "FreeRTOS.h"
#include "event_groups.h"

#define AVG_SLOPE (4.3F)
#define V_AT_25C  (1.43F)
#define V_REF_INT (1.2F)

extern uint16_t ADC_RESULT[2];
extern EventGroupHandle_t event_uart_tx;

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	BaseType_t	xHigherPriorityTaskWoken = pdFALSE;
	xEventGroupSetBitsFromISR(event_uart_tx, MQTT_PUBLISH_BIT,&xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	HAL_ADC_Stop_DMA(hadc);
}

float calculate_temperature(void)
{
	float V_Sense, V_Ref;
	float Temperature;

	ADC_RESULT[0] = calibrate_adc(ADC_RESULT[0]);
	ADC_RESULT[1] = calibrate_adc(ADC_RESULT[1]);
	V_Ref = (float)((V_REF_INT * 4095.0)/ADC_RESULT[0]);
	V_Sense = (float)(ADC_RESULT[1] * V_Ref) / 4095.0;
	Temperature = (((V_AT_25C - V_Sense) * 1000.0) /AVG_SLOPE) + 25.0;

	return Temperature;
}
