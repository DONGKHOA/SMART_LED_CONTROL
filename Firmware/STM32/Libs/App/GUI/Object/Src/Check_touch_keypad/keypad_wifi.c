/*
 * keypad_MQTT.c
 *
 *  Created on: Apr 15, 2024
 *      Author: ACER
 */

#include "Keypad/keypad_wifi.h"

static const uint16_t keypad_wifi[12][4] = {
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

key_character_t check_event_keypad_Wifi(int16_t x, int16_t y)
{
	key_character_t i;
	for (i = 0; i < 12; i++)
	{
		if (x > keypad_wifi[i][0] && x < keypad_wifi[i][1] && y > keypad_wifi[i][2] && keypad_wifi[i][3])
			break;
	}
	return i;
}
