#include "event.h"
#include "check_touch_screen/check_touch_screen.h"
#include "screen.h"
#include "graphics.h"
#include "queue.h"
#include "main.h"


extern control_led_t led_state;
extern control_auto_t auto_state;

extern int16_t x;
extern int16_t y;
extern uint8_t flag_is_touch;

extern QueueHandle_t queue_control_led;
extern QueueHandle_t queue_control_auto;

void check_event_screen_4(screen_state_t *screen)
{
	touch_icon_screen4_t touch = check_event_icon_screen4(x, y);
	if (touch != NO_TOUCH_ICON_SC4)
	{
		if (flag_is_touch == 0)
		{
			if (touch == ICON_RETURN_SC4) /*return screen_1 */
			{
				bit_map_screen_1.screen = 1;
				bit_map_screen_1.wifi = 1;
				bit_map_screen_1.home = 1;
				bit_map_screen_1.MQTT = 1;
				*screen = SCREEN_START;
			}
			else if (touch == ICON_CONTROL) /*Turn on the light or turn off the light if icon_control is pressed and display the led status*/
			{
				if (led_state == LED_OFF) led_state = LED_ON;
				else led_state = LED_OFF;

				xQueueSend(queue_control_led, &led_state, portMAX_DELAY);

				if(led_state == LED_ON)
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
				if (auto_state == AUTO_OFF) auto_state = AUTO_ON;
				else auto_state = AUTO_OFF;

//				xQueueSend(queue_control_auto, &auto_state, portMAX_DELAY);
				if (auto_state == AUTO_ON)
				{
					bit_map_screen_4.on_auto = 1;
				}
				else
				{
					bit_map_screen_4.off_auto = 1;
				}
				// draw one note to display on or off mode auto
			}
			flag_is_touch = 1;
		}
		x = 1000;
		y = 1000;
	}
	else
	{
		flag_is_touch = 0;
	}
}
