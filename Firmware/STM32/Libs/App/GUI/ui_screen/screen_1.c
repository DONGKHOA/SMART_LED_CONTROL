/*
 * screen_1.c
 *
 *  Created on: Mar 17, 2024
 *      Author: Author: vuhuy
 */
#include "screen.h"
#include "Icon/icon.h"
#include "graphics.h"

field_bit_screen1_t bit_map_screen_1 = {
	.screen = 1,
	.wifi = 1,
	.home = 1,
	.MQTT = 1,
};

/**
 * The function `screen_1` clears the screen and displays different icons based on the values of
 * corresponding bit_map_screen_1 flags.
 */
void screen_1(void)
{
	if (bit_map_screen_1.screen == 1)
	{
		GraphicsClear(WHITE);
		bit_map_screen_1.screen = 0;
	}
	if (bit_map_screen_1.wifi == 1)
	{
		GraphicsColourBitmap(99, 75, 41, 39, icon_wifi);
		bit_map_screen_1.wifi = 0;
	}
	if (bit_map_screen_1.home == 1)
	{
		GraphicsColourBitmap(95, 154, 50, 48, icon_home);
		bit_map_screen_1.home = 0;
	}
	if (bit_map_screen_1.MQTT == 1)
	{
		GraphicsColourBitmap(94, 150, 216, 255, icon_MQTT);
		bit_map_screen_1.MQTT = 0;
	}
}
