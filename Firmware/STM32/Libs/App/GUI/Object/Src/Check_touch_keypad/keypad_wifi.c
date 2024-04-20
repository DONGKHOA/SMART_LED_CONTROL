/*
 * keypad_MQTT.c
 *
 *  Created on: Apr 15, 2024
 *      Author: ACER
 */

#include "Keypad/keypad_wifi.h"

static const uint16_t keypad_wifi[13][4] = {
	{50, 85, 125, 160}, //0
	{103, 138, 125, 160}, //1
	{155, 190, 125, 160}, //2
	{50, 85, 170, 205}, //3
	{103, 138, 170, 205}, //4
	{155, 190, 170, 205}, //5
	{50, 85, 215, 250}, //6
	{103, 138, 215, 250}, //7
	{155, 190, 215, 250}, //8
	{52, 263, 82, 292}, //9
	{103, 138, 260, 295}, //x
	{156, 263, 189, 296}, //v
	{7, 35, 8, 31}, // icon return
};

key_character_t check_event_keypad_Wifi(int16_t x, int16_t y)
{
	key_character_t i;
	for (i = 0; i < 13; i++)
	{
		if (x > keypad_wifi[i][0] && x < keypad_wifi[i][1] && y > keypad_wifi[i][2] && keypad_wifi[i][3])
			break;
	}
	return i;
}
