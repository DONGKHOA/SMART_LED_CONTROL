#include "event.h"
#include "Keypad/keypad.h"

#define OFFSET_X 15
#define OFFSET_Y 90

extern int16_t x;
extern int16_t y;

static const char character_key[12] = {'0', '1', '2', '3',
										'4', '5', '6', '7',
										'8', '9', 'x', 'v'};

static uint8_t password[9];
static uint8_t password_pos = 0;

void check_event_screen_3(check_event_t *event, 
							screen_state_t *screen)
{
	static uint16_t x_coordinate = 0;
	
	key_character_t key = check_event_keypad(x, y);
	if (key != NO_TOUCH )
	{
		if (key == NUM_x)
		{
			password_pos = 0;
			*event = EVENT_SCREEN_3;
			x_coordinate  = 0;
		}
		else if (key == NUM_v)
		{
			uint8_t temp;
			password_pos = 0;
			*event = EVENT_SCREEN_3;
			xQueueSend(queue_data_tx, &password, 0);
			xQueueReceive(queue_data_rx, &temp, portMAX_DELAY);
			// check success or unsuccess
			x_coordinate  = 0;
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
