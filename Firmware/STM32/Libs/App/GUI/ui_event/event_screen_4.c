#include "event.h"
#include "check_touch_screen/check_touch_screen.h"
#include "screen.h"
#include "graphics.h"

extern int16_t x;
extern int16_t y;
// extern field_bit_screen4_t bit_map_screen_4;

void check_event_screen_4(check_event_t *event, 
							screen_state_t *screen)
{
	touch_icon_screen4_t touch = check_event_icon_screen4(x, y);
	if(touch != NO_TOUCH_ICON_SC4)
	{
		if( touch == ICON_RETURN_SC4 )
		{
			*event = EVENT_SCREEN_1;
			*screen = SCREEN_START;
		}
		else if( touch == ICON_CONTROL )
		{
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7))
			{
				bit_map_screen_4.ON = 1;  // if Button on
			}
			else 
			{
				bit_map_screen_4.OFF = 1;  // if Button off
			}
		}
		else
		{
			// set mode auto turn on or off led follow lux
		}
	}
}
