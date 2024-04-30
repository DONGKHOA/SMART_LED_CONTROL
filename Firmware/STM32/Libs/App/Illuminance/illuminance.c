#include "illuminance.h"


float volt;
float Ev_before;
int16_t var;
int16_t var_after;
int16_t Ev;
extern ADC_HandleTypeDef hadc2;
extern uint8_t autocontrol;
int low_threshold = 10;
int high_threshold = 875;
extern volatile uint8_t led_state; 
extern uint8_t check_state_auto;

extern QueueHandle_t queue_control_led;

float voltage_adc()
{
	var = read_adc(&hadc2);
	var_after = calibrate_adc(var);
	volt = (((float)var_after*3.3)/4096);
 	volt = volt/6;
	return volt;
}

float illuminance_adc()
{
	float R = volt*10; // I=100uA, (The unit of R is KOhm)
	Ev_before = R - 4.6974;
	Ev_before = Ev_before/(-1.02*10e-4);
	Ev_before = Ev_before/5 ;
	if (Ev_before < 800)   Ev_before= Ev_before - 500;

	if (Ev_before < low_threshold)
	    {
	        Ev_before = low_threshold;
	    }
	else if (Ev_before > high_threshold)
	    {
	        Ev_before = 1000;
	    }
	return Ev_before;
}

int16_t adjust_Ev()
{
    Ev = round((double)Ev_before / 10) * 10;
    if (Ev < low_threshold)
    {
        Ev = low_threshold;
    }
    else if (Ev > high_threshold)
    {
        Ev = 1000;
    }
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
	led_state = 1;
}

void turnOffLight()
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
	led_state = 0;
}

void autocontrol_mode()
{
	if (xQueueReceive(queue_control_led, &check_state_auto, portMAX_DELAY) == pdPASS)
	{
		if (illuminance_signal() )
		turnOnLight();
		else turnOffLight();
	}
	else turnOffLight();
}
