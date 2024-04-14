/*
 * keypad.c
 *
 *  Created on: Apr 14, 2024
 *      Author: ACER
 */

#include "Keypad/keypad.h"
#include "Icon/icon.h"
#include "graphics.h"
#include "touch.h"
static const uint16_t keypad[12][4] = {
		{50, 85, 125, 160},
		{103, 138, 125, 160},
		{155, 190, 125, 160},
		{50, 85, 170, 205},
		{103, 138, 170, 205},
		{155, 190, 170, 205},
		{50, 85, 215, 250},
		{103, 138, 215, 250},
		{155, 190, 215, 250},
		{52, 263, 82, 292},
		{103, 138, 260, 295},
		{156, 263, 189, 296},
};

key_character_t check_event_keypad( int16_t x,  int16_t y)
{
	key_character_t i;
	for(i = 0; i < 12; i++)
	{
		if( x > keypad[i][0] && x < keypad[i][1] && y > keypad[i][2] && keypad[i][3] )
			break;
	}
	return i;
}
