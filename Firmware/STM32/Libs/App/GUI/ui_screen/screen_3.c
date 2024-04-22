/*********************
 *      INCLUDES
 *********************/

#include "screen.h"
#include "Keypad/keypad_wifi.h"
#include "graphics.h"
#include "icon.h"

/**********************
 *     VARIABLES
 **********************/

field_bit_screen3_t bit_map_screen_3;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void screen_3(void)
{
	if (bit_map_screen_3.screen == 1)
	{
		GraphicsClear(WHITE);
		bit_map_screen_3.screen = 0;
	}

	if (bit_map_screen_3.ret == 1)
	{
		GraphicsColourBitmap(7, 8, 28, 23, icon_return);
		bit_map_screen_3.ret = 0;
	}

	if (bit_map_screen_3.text == 1)
	{
		GraphicsLargeString(35, 45, "ENTER PASSWORD", BLACK);
		bit_map_screen_3.text = 0;
	}

	if (bit_map_screen_3.frame == 1)
	{
		GraphicsRoundedRectangle(35, 82, 170, 30, 5, BLACK);
		bit_map_screen_3.frame = 0;
	}

	if (bit_map_screen_3.key == 1)
	{
		object_keypad_Wifi();
		bit_map_screen_3.key = 0;
	}
}
