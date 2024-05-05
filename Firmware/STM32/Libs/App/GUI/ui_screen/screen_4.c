/*********************
 *      INCLUDES
 *********************/

#include "screen.h"
#include "graphics.h"
#include "Icon/icon.h"

field_bit_screen4_t bit_map_screen_4;

void screen_4(EventBits_t uxBits)
{
	if (bit_map_screen_4.screen == 1)
	{
		GraphicsClear(WHITE);
		bit_map_screen_4.screen = 0;
	}

	if (bit_map_screen_4.ret == 1)
	{
		GraphicsColourBitmap(7, 8, 28, 23, icon_return);
		bit_map_screen_4.ret = 0;
	}

	if (bit_map_screen_4.control == 1)
	{
		GraphicsColourBitmap(91, 63, 59, 60, icon_button);
		bit_map_screen_4.control = 0;
	}

	if (bit_map_screen_4.automode == 1)
	{
		GraphicsColourBitmap(89, 176, 61, 60, icon_auto);
		bit_map_screen_4.automode = 0;
	}

	if(bit_map_screen_4.on_auto == 1)
	{
		GraphicsLargeString(94, 236, "AUTO", BLACK);
		bit_map_screen_4.on_auto = 0;
	}

	if(bit_map_screen_4.off_auto == 1)
	{
		GraphicsLargeString(94, 236, "AUTO", WHITE);
		bit_map_screen_4.off_auto = 0;
	}

	if (bit_map_screen_4.ON == 1)
	{
		GraphicsLargeString(108, 44, "ON", BLACK);
		GraphicsLargeString(108, 29, "OFF", WHITE);
		bit_map_screen_4.ON = 0;
	}

	if (bit_map_screen_4.OFF == 1)
	{
		GraphicsLargeString(108, 29, "OFF", BLACK);
		GraphicsLargeString(108, 44, "ON", WHITE);
		bit_map_screen_4.OFF = 0;
	}
}
