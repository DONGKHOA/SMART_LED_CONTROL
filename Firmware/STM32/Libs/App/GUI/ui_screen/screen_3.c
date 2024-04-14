/*
 * screen_3.c
 *
 *  Created on: Mar 17, 2024
 *      Author: dongkhoa
 */
#include "screen.h"
#include "Keypad/keypad.h"

typedef struct 
{
	uint8_t screen : 1;
	uint8_t ret : 1;
	uint8_t text : 1;
	uint8_t frame: 1;
	uint8_t key : 1;
} field_bit_sreen_t;

static field_bit_sreen_t bit_map = {
	.frame = 1,
	.ret = 1,
	.key = 1,
	.text = 1,
	.screen = 1,
};

void screen_3(void)
{
	if (bit_map.screen == 1)
	{
		GraphicsClear(WHITE);	
		bit_map.screen = 0;
	}

	if (bit_map.ret == 1)
	{
		GraphicsColourBitmap(7, 8, 28, 23, icon_return);
		bit_map.ret = 0;	
	}

	if (bit_map.text == 1)
	{
		GraphicsLargeString(35, 45, "ENTER PASSWORD", BLACK);
		bit_map.text = 0;	
	}

	if (bit_map.frame == 1)
	{
		GraphicsRoundedRectangle(35, 82, 170, 30, 5, BLACK);
		bit_map.frame = 0;	
	}

	if (bit_map.key == 1)
	{
		object_Keypad();
		bit_map.key = 0;
	}
}
