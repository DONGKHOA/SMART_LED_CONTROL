/**
 * The function `screen_2` sets up a graphical user interface for displaying Wi-Fi network information.
 */
/*
 * screen_2.c
 *
 *  Created on: Mar 17, 2024
 *      Author: Author: vuhuy
 */
#include "screen.h"
#include "Icon/icon.h"
#include "graphics.h"

field_bit_screen2_t bit_map_screen_2 = {
	.screen = 1,
	.ret = 1,
	.text1 = 1,
	.text2 = 1,
	// .WIFI_Connected = 1,
	.WIFI1 = 1,
	.WIFI2 = 1,
	.WIFI3 = 1,
	.WIFI4 = 1,
	.WIFI5 = 1,
};

void screen_2(void)
{
	if (bit_map_screen_2.screen == 1)
	{
		GraphicsClear(WHITE);
		GraphicsRoundedRectangle(34, 54, 170, 25, 5, BLACK);
		bit_map_screen_2.screen = 0;
	}

	if (bit_map_screen_2.ret == 1)
	{
		GraphicsColourBitmap(5, 7, 28, 23, icon_return);
		bit_map_screen_2.ret = 0;
	}

	if (bit_map_screen_2.text1 == 1)
	{
		GraphicsLargeString(80, 25, "Wi-Fi", BLACK);
		bit_map_screen_2.text1 = 0;
	}

	if (bit_map_screen_2.text2 == 1)
	{
		GraphicsLargeString(5, 92, "Wi-Fi Network", BLACK);
		GraphicsHline(0, 240, 90, BLACK);
		bit_map_screen_2.text2 = 0;
	}

	if (bit_map_screen_2.WIFI_Connected == 1)
	{
		GraphicsRoundedRectangle(34, 54, 170, 25, 5, BLACK);
		bit_map_screen_2.WIFI_Connected = 0;
	}

	if (bit_map_screen_2.WIFI1 == 1)
	{
		GraphicsRoundedRectangle(34, 117, 170, 25, 5, BLACK);
		bit_map_screen_2.WIFI1 = 0;
	}

	if (bit_map_screen_2.WIFI2 == 1)
	{
		GraphicsRoundedRectangle(34, 156, 170, 25, 5, BLACK);
		bit_map_screen_2.WIFI2 = 0;
	}

	if (bit_map_screen_2.WIFI3 == 1)
	{
		GraphicsRoundedRectangle(34, 195, 170, 25, 5, BLACK);
		bit_map_screen_2.WIFI3 = 0;
	}

	if (bit_map_screen_2.WIFI4 == 1)
	{
		GraphicsRoundedRectangle(34, 234, 170, 25, 5, BLACK);
		bit_map_screen_2.WIFI4 = 0;
	}

	if (bit_map_screen_2.WIFI5 == 1)
	{
		GraphicsRoundedRectangle(34, 273, 170, 25, 5, BLACK);
		bit_map_screen_2.WIFI5 = 0;
	}
}
