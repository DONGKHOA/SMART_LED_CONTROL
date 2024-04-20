/*
 * screen_4.c
 *
 *  Created on: Mar 17, 2024
 *      Author: Author: vuhuy
 */
#include "screen.h"
#include "graphics.h"
#include "Icon/icon.h"

field_bit_screen4_t bit_map_screen_4 = {
	.screen = 1,
	.control = 1,
	.ret = 1,
	.automode = 1,
	.ON = 0,
	.OFF = 0,
};

void screen_4(void)
{
	if(bit_map_screen_4.screen == 1)
	{
		GraphicsClear(WHITE);
		bit_map_screen_4.screen = 0;
	}

	if(bit_map_screen_4.ret == 1)
	{
		GraphicsColourBitmap(7, 8, 28, 23, icon_return);
		bit_map_screen_4.ret = 0;
	}

	if(bit_map_screen_4.control == 1)
	{
		GraphicsColourBitmap(91, 134, 59, 60, icon_button);
		bit_map_screen_4.control = 0;
	}

	if(bit_map_screen_4.automode == 1)
	{
		GraphicsColourBitmap(89, 176, 61, 60, icon_auto);
		bit_map_screen_4.automode = 0;
	}

	if(bit_map_screen_4.ON == 1)
	{
		GraphicsLargeString(95, 119, "ON", BLACK);
		GraphicsRectangle(126,119, 20,15, WHITE);
	}

	if(bit_map_screen_4.OFF == 1)
	{
		GraphicsLargeString(126, 119, "OFF", BLACK);
		GraphicsRectangle(95, 119, 20, 15, WHITE);
	}
}
