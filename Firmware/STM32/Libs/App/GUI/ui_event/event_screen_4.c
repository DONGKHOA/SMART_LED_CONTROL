#include "event.h"
#include "check_touch_screen/check_touch_screen.h"
#include "screen.h"
#include "graphics.h"
#include "queue.h"

extern int16_t x;
extern int16_t y;
extern QueueHandle_t queue_control_led;
uint8_t autocontrol = 0;
volatile uint8_t led_state = 0; 
volatile uint8_t auto_control = 0;
 


void check_event_screen_4(screen_state_t *screen)
{
	touch_icon_screen4_t touch = check_event_icon_screen4(x, y);
	if (touch != NO_TOUCH_ICON_SC4)
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
			uint8_t check_state_led = (led_state == 0) ? 1 : 0; 
			xQueueSend(queue_control_led, &check_state_led, portMAX_DELAY);
			if(check_state_led)
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
			uint8_t check_state_auto = (auto_control == 0) ? 1 : 0;
			xQueueSend(queue_control_led, &check_state_auto, portMAX_DELAY);
			if (check_state_auto)
			{
				bit_map_screen_4.on_auto = 1;	
			}
			else
			{
				bit_map_screen_4.off_auto = 1;
			}
			// draw one note to display on or off mode auto
		}
	}
}
