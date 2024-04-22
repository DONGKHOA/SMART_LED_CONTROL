#include "event.h"
#include "check_touch_screen/check_touch_screen.h"
#include "screen.h"
#include "graphics.h"

extern int16_t x;
extern int16_t y;
uint8_t autocontrol = 0;

void check_event_screen_4(screen_state_t *screen)
{
	touch_icon_screen4_t touch = check_event_icon_screen4(x, y);
	if (touch != NO_TOUCH_ICON_SC4)
	{
		if (touch == ICON_RETURN_SC4) /*return screen_1 */
		{
			*screen = SCREEN_START;
		}
		else if (touch == ICON_CONTROL) /*Turn on the light or turn off the light if icon_control is pressed and display the led status*/
		{
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);
			if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7))
			{
				bit_map_screen_4.ON = 1; // if Button on
			}
			else
			{
				bit_map_screen_4.OFF = 1; // if Button off
			}
		}
		else /*Automatically turn on or off the light according to brightness if pressed icon_auto*/
		{
			vTaskDelay(2000);
			if (touch == ICON_AUTO)
			{
				autocontrol = 1; // turn on mode auto
			}
			if (touch == ICON_AUTO)
			{
				vTaskDelay(1000);
				if (touch == ICON_AUTO)
				{
					autocontrol = 0; // turn off mode auto
				}
			}
			// draw one note to display on or off mode auto
		}
	}
}
