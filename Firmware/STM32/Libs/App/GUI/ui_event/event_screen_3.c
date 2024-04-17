#include "event.h"
#include "Keypad/keypad_wifi.h"

#define OFFSET_X 15
#define OFFSET_Y 90

extern int16_t x;
extern int16_t y;

static const char character_key[12] = {'0', '1', '2', '3',
									   '4', '5', '6', '7',
									   '8', '9', 'x', 'v'};

static uint8_t password[9];
static uint8_t password_pos = 0;
static uint8_t full = 8;
uint8_t connected = 0; // check wifi have been connected yet ?

void check_event_screen_3(check_event_t *event,
						  screen_state_t *screen)
{
	static uint16_t x_coordinate = 0;

	key_character_t key = check_event_keypad_Wifi(x, y);
	if (key != NO_TOUCH)
	{
		if (key == NUM_x)
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
		else if (key == NUM_v)
		{
			if (password_pos > full)
			{
				uint8_t temp;
				password_pos = 0;
				*event = EVENT_SCREEN_3;
				xQueueSend(queue_data_tx, &password, 0);
				xQueueReceive(queue_data_rx, &temp, portMAX_DELAY);
				// check success or unsuccess, if success set *screen = screen_4
				// declare one variable with name ' connected ' if wifi is connected
				x_coordinate = 0;
				for (password_pos = 0; password_pos < 9; password_pos++)
				{
					password[password_pos] = 0;
				}
			}
		}
		else
		{
			x_coordinate += OFFSET_X;
			password[password_pos] = character_key[key];
			password_pos++;
			GraphicsLargeCharacter(x_coordinate, OFFSET_Y,
								   character_key[key], WHITE);
		}
	}
}
