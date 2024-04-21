#include "event.h"
#include "Keypad/keypad_wifi.h"

#define OFFSET_X 15
#define OFFSET_Y 90

extern int16_t x;
extern int16_t y;

static const char character_key[12] = {'0', '1', '2', '3',
									   '4', '5', '6', '7',
									   '8', '9', 'x', 'v'};

uint8_t password[8];
static uint8_t password_pos = 0;
static uint8_t full = 8;
uint8_t connected = 0; /*The variable contains the status of whether wifi is connected or not*/

void check_event_screen_3(check_event_t *event,
						  screen_state_t *screen)
{
	static uint16_t x_coordinate = 0; /*variable containing location*/

	key_character_t key = check_event_keypad_Wifi(x, y); /*checking touch position of each element in the keyboard */
	if (key != NO_TOUCH)
	{
		if (key == NUM_x) /*If you press delete, delete all elements entered into the password[] array,
						  redraw the password display frame, and check screen events again 3*/
		{
			for (password_pos = 0; password_pos < 9; password_pos++)
			{
				password[password_pos] = 0;
			}
			bit_map_screen_3.frame = 1;
			password_pos = 0;
			*event = EVENT_SCREEN_3;
			x_coordinate = 0;
		}

		else if (key == ICON_RETURN) /*return to screen_2*/
		{
			for (password_pos = 0; password_pos < 8; password_pos++)
			{
				password[password_pos] = 0;
			}
			*event = EVENT_SCREEN_2;
			*screen = EVENT_SCREEN_2;
		}

		else if (key == NUM_v) /*If you click check, check to see if the entered password is sufficient.
								   If so, write it to Queue to check*/
		{
			if (password_pos > full)
			{
				uint8_t temp;
				password_pos = 0;
				*event = EVENT_SCREEN_3;
				/* add '2ssid\rpassword\r\n' into a array to transmit, heading will be
				send first and data will be second */
				xQueueSend(queue_data_tx, &password, 0);
				xQueueReceive(queue_data_rx, &temp, portMAX_DELAY);
				// check success or unsuccess, if success set *screen = screen_5
				x_coordinate = 0;
				for (password_pos = 0; password_pos < 8; password_pos++)
				{
					password[password_pos] = 0;
				}
			}
		}

		else /*touch keypad to entering password*/
		{
			x_coordinate += OFFSET_X;
			password[password_pos] = character_key[key];
			password_pos++;
			GraphicsLargeCharacter(x_coordinate, OFFSET_Y,
								   character_key[key], WHITE);
		}
	}
}
