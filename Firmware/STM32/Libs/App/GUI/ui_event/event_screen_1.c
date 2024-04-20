#include "event.h"
#include "check_touch_screen/check_touch_screen.h"
#include "screen.h"

/**
 * The function `check_event_screen_1` checks for a touch event on a specific icon and updates the
 * event and screen state accordingly.
 *
 * @param event The `event` parameter is a pointer to a variable of type `check_event_t`, which is used
 * to store information about the current event being checked.
 * @param screen The `screen` parameter is a pointer to a `screen_state_t` structure, which likely
 * represents the current state of the screen in the application.
 */
extern int16_t x;
extern int16_t y;
extern uint8_t connected;

void check_event_screen_1(check_event_t *event,
						  screen_state_t *screen)
{
	touch_icon_screen1_t touch = check_event_icon_screen1(x, y);
	if (touch != NO_TOUCH_ICON_SC1)
	{
		if (touch == ICON_WIFI) // touch icon wifi
		{
			/* add string following frame '1ON'
			*/
			*event = EVENT_SCREEN_2;
			*screen = SCREEN_WIFI;
		}
		else if (touch == ICON_HOME)
		{
			if (connected == 1) // wifi connected
			{
				*event = EVENT_SCREEN_4;
				*screen = SCREEN_MAIN;
			}
			else
			{
				*event = EVENT_SCREEN_2;
				*screen = SCREEN_WIFI;
			}
		}
		else
		{
			if(connected == 1) // wifi connected
			{
			*event = EVENT_SCREEN_5;
			*screen = SCREEN_MQTT;
			}
		}
	}
}
