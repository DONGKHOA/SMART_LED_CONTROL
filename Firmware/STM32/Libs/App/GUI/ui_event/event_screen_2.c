#include "event.h"
#include "check_touch_screen/check_touch_screen.h"
#include "screen.h"

extern int16_t x;
extern int16_t y;

void check_event_screen_2(screen_state_t *screen)
{
	touch_icon_screen2_t touch = check_event_icon_screen2(x, y);
	if (touch != NO_TOUCH_ICON_SC2)
	{
		if (touch == ICON_RETURN) /*if touch icon_return -> return to screen_1*/
		{
			bit_map_screen_1.screen = 1;
			bit_map_screen_1.wifi = 1;
			bit_map_screen_1.home = 1;
			bit_map_screen_1.MQTT = 1;
			*screen = SCREEN_START;
		}
	}
}
