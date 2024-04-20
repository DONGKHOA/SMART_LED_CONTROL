
#include "cal_temperature.h"
#include "stdint.h"
#include "calibrate_adc.h"


uint8_t UpdateEvent;
extern uint16_t ADC_RESULT[2];
float V_Sense, V_Ref;


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    // Xử lý khi quá trình chuyển đổi ADC hoàn thành
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
	UpdateEvent = 1;
}

float calculate_temperature(void)
{
    // Tính toán nhiệt độ từ dữ liệu ADC và trả về kết quả
	ADC_RESULT[0] = calibrate_adc(ADC_RESULT[0]);
	ADC_RESULT[1] = calibrate_adc(ADC_RESULT[1]);
	V_Ref = (float)((V_REF_INT * 4095.0)/ADC_RESULT[0]);
	V_Sense = (float)(ADC_RESULT[1] * V_Ref) / 4095.0;
	float Temperature = (((V_AT_25C - V_Sense) * 1000.0) /AVG_SLOPE) + 25.0;
	return Temperature;
	UpdateEvent = 0;
}
