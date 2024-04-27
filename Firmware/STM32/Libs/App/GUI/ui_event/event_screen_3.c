/*********************
 *      INCLUDES
 *********************/

#include "event.h"
#include "Keypad/keypad_wifi.h"
#include <string.h>
#include "main.h"

/*********************
 *      DEFINES
 *********************/

#define OFFSET_X 				15
#define OFFSET_Y 				90
#define MIN_LENGTH_PASSWORD		8

/**********************
 *  STATIC VARIABLES
 **********************/

static const char character_key[12] = {'0', '1', '2', '3',
									   '4', '5', '6', '7',
									   '8', '9', 'x', 'v'};
static uint8_t password_pos = 0;
static char password[9];

/**********************
 *  EXTERN VARIABLES
 **********************/

extern int16_t x;
extern int16_t y;

extern uint8_t buffer_uart_tx[RX_BUFFER_SIZE + 1];
extern EventGroupHandle_t event_uart_rx;
extern EventGroupHandle_t event_uart_tx;

extern char text[18];
extern EventBits_t bitsScreen3;

extern char ssid_connect[32];

/**********************
 *     VARIABLES
 **********************/

char ssid[32];

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void check_event_screen_3(screen_state_t *screen)
{
	static uint16_t x_coordinate = 0; /*variable containing location*/

	key_character_t key = check_event_keypad_Wifi(x, y); /*checking touch position of each element in the keyboard */
	if (key != NO_TOUCH)
	{
		if (key == NUM_x)
		{
			memset(password, '\0', sizeof(password));
			bit_map_screen_3.frame = 1;
			password_pos = 0;
			x_coordinate = 0;
		}

		else if (key == ICON_RETURN) /*return to screen_2*/
		{
			memset(password, '\0', sizeof(password));
			password_pos = 0;
			x_coordinate = 0;

			bit_map_screen_2.screen = 1;
			bit_map_screen_2.ret = 1;
			bit_map_screen_2.on_off_wifi = 1;
			bit_map_screen_2.text1 = 1;
			bit_map_screen_2.text2 = 1;
			bit_map_screen_2.WIFI1 = 1;
			bit_map_screen_2.WIFI2 = 1;
			bit_map_screen_2.WIFI3 = 1;
			bit_map_screen_2.WIFI4 = 1;
			bit_map_screen_2.WIFI5 = 1;
			bit_map_screen_2.NEXT = 1;
			*screen = SCREEN_WIFI;
		}

		else if (key == NUM_v) /*If you click check, check to see if the entered password is sufficient.
								   If so, write it to Queue to check*/
		{
			if (password_pos >= (MIN_LENGTH_PASSWORD - 1))
			{
				password_pos = 0;
				x_coordinate = 0;
				sprintf((char *)buffer_uart_tx, "%s\r%s", ssid, password);
				xEventGroupSetBits(event_uart_tx, CONNECT_WIFI_BIT);
				memset(password, '\0', sizeof(password));
			}
			else
			{
				bit_map_screen_3.text = 1;
				strcpy(text, "Min 8 number");
			}
		}

		else /*touch keypad to entering password*/
		{
			password[password_pos] = character_key[key];
			GraphicsLargeCharacter(x_coordinate, OFFSET_Y,
								   character_key[key], WHITE);
			password_pos++;
			x_coordinate += OFFSET_X;
		}
	}

	if (bitsScreen3 & CONNECT_WIFI_SUCCESSFUL_BIT)
	{
		strcpy(ssid_connect, ssid);
		bit_map_screen_1.connected_WIFI = 1;
		bit_map_screen_2.screen = 1;
		bit_map_screen_2.ret = 1;
		bit_map_screen_2.on_off_wifi = 1;
		bit_map_screen_2.WIFI_Connected = 1;
		bit_map_screen_2.text1 = 1;
		bit_map_screen_2.text2 = 1;
		bit_map_screen_2.WIFI1 = 1;
		bit_map_screen_2.WIFI2 = 1;
		bit_map_screen_2.WIFI3 = 1;
		bit_map_screen_2.WIFI4 = 1;
		bit_map_screen_2.WIFI5 = 1;
		bit_map_screen_2.NEXT = 1;
		*screen = SCREEN_WIFI;
	}
}
