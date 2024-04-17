#include "event.h"
#include "check_touch_screen.h"
#include "screen.h"

extern int16_t x;
extern int16_t y;

void check_event_screen_2(check_event_t *event, 
							screen_state_t *screen)
{
	touch_icon_screen2_t touch = check_event_icon_screen2(x, y);
	if(touch != NO_TOUCH_ICON_SC2)
	{
		if(touch == ICON_RETURN)
		{
			*event = EVENT_SCREEN_1;
			*screen = SCREEN_START;
		}
		else if(touch == WIFI1)
		{
			*event = EVENT_SCREEN_3;
			*screen = SCREEN_KEYPAD;
			// send to ESP32 "STM32 have choosen WIFI1"
		}
		else if(touch == WIFI2)
		{
			*event = EVENT_SCREEN_3;
			*screen = SCREEN_KEYPAD;
			// send to ESP32 "STM32 have choosen WIFI2"
		}
		else if(touch == WIFI3)
		{
			*event = EVENT_SCREEN_3;
			*screen = SCREEN_KEYPAD;
			// send to ESP32 "STM32 have choosen WIFI3"
		}
		else if(touch == WIFI4)
		{
			*event = EVENT_SCREEN_3;
			*screen = SCREEN_KEYPAD;
			// send to ESP32 "STM32 have choosen WIFI4"
		}
		else 
		{
			*event = EVENT_SCREEN_3;
			*screen = SCREEN_KEYPAD;
			// send to ESP32 "STM32 have choosen WIFI5"
		}
	}

}
