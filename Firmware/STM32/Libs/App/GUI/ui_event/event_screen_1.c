/*********************
 *      INCLUDES
 *********************/

#include "event.h"
#include "check_touch_screen/check_touch_screen.h"
#include "screen.h"

extern int16_t x;
extern int16_t y;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * The function `check_event_screen_1` checks for a touch event on a specific icon and updates the
 * event and screen state accordingly.
 *
 * @param event The `event` parameter is a pointer to a variable of type `check_event_t`, which is used
 * to store information about the current event being checked.
 * @param screen The `screen` parameter is a pointer to a `screen_state_t` structure, which likely
 * represents the current state of the screen in the application.
 */

void check_event_screen_1(screen_state_t *screen)
{
	touch_icon_screen1_t touch = check_event_icon_screen1(x, y);
	if (touch != NO_TOUCH_ICON_SC1)
	{
		if (touch == ICON_WIFI) /*if touch icon wifi*/
		{
			*screen = SCREEN_WIFI;
		}
		else if (touch == ICON_HOME) /*if touch icon home*/
		{
			*screen = SCREEN_MAIN;
		}
		else /*if touch icon MQTT*/
		{
			*screen = SCREEN_MQTT;
		}
	}
}
