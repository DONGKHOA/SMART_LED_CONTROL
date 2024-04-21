/**
 * The function `screen_5` updates the display with a frame, return icon, text, and keypad for entering
 * a password.
 */
/*
 * screen_5.c
 *
 *  Created on: Apr 15, 2024
 *      Author: vuhuy
 */
#include "screen.h"
#include "Keypad/keypad_MQTT.h"
#include "graphics.h"
#include "icon.h"

field_bit_screen5_t bit_map_screen_5 = {
	.frame = 1,
	.ret = 1,
	.key = 1,
	.text = 1,
	.screen = 1,
};

void screen_5(void)
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
