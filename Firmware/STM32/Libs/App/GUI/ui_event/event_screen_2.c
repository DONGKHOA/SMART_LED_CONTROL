/*********************
 *      INCLUDES
 *********************/

#include "event.h"
#include "check_touch_screen/check_touch_screen.h"
#include "screen.h"
#include "main.h"
#include "FreeRTOS.h"
#include "event_groups.h"

/**********************
 *  EXTERN VARIABLES
 **********************/

extern int16_t x;
extern int16_t y;

extern uint8_t buffer_uart_rx[RX_BUFFER_SIZE + 1];
extern uint8_t buffer_uart_tx[RX_BUFFER_SIZE + 1];
extern EventGroupHandle_t event_uart_tx;
extern uint8_t state_wifi;
extern uint8_t numPage;
extern uint8_t limitNumPage;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/


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
		else if (touch == ON_OFF_WIFI)
		{
			bit_map_screen_2.on_off_wifi = 1;
			state_wifi = !state_wifi;
			if (state_wifi == 1)
			{
				xEventGroupSetBits(event_uart_tx, ON_WIFI_BIT);
			}
			else
			{
				xEventGroupSetBits(event_uart_tx, OFF_WIFI_BIT);
			}
		}
		else if (touch == NEXT)
		{
			if(numPage < limitNumPage) numPage++;
		}
		else if (touch == BACK)
		{
			if(numPage > 0) numPage--;
		}
		else if (touch == WIFI1)
		{
			// chuyeenr qua man hinh 3
		}
		else if (touch == WIFI2)
		{
			// chuyeenr qua man hinh 3
		}
		else if (touch == WIFI3)
		{
			// chuyeenr qua man hinh 3
		}
		else if (touch == WIFI4)
		{
			// chuyeenr qua man hinh 3
		}
		else if (touch == WIFI5)
		{
			// chuyeenr qua man hinh 3
		}
	}
}
