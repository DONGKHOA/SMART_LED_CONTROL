/*********************
 *      INCLUDES
 *********************/

#include "screen.h"
#include "Keypad/keypad_MQTT.h"
#include "graphics.h"
#include "icon.h"

field_bit_screen5_t bit_map_screen_5;

void screen_5(EventBits_t uxBits)
{
	if (bit_map_screen_5.screen == 1)
	{
		GraphicsClear(WHITE);
		bit_map_screen_5.screen = 0;
	}

	if (bit_map_screen_5.frame == 1)
	{
		GraphicsRoundedRectangle(35, 82, 170, 30, 5, BLACK);
		bit_map_screen_5.frame = 0;
	}

	if (bit_map_screen_5.ret == 1)
	{
		GraphicsColourBitmap(7, 8, 28, 23, icon_return);
		bit_map_screen_5.ret = 0;
	}

	if (bit_map_screen_5.text == 1)
	{
		GraphicsLargeString(35, 45, "ENTER PASSWORD", BLACK);
		bit_map_screen_5.text = 0;
	}

	if (bit_map_screen_5.key == 1)
	{
		object_Keypad_MQTT();
		bit_map_screen_5.key = 0;
	}
}
