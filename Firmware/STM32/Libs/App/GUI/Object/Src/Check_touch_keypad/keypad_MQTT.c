#include "Keypad/keypad_MQTT.h"

/* position of each element in the keyboard */
static const uint16_t keypad_MQTT[14][4] = {
	{50, 85, 125, 160},	  // 0
	{103, 138, 125, 160}, // 1
	{155, 190, 125, 160}, // 2
	{50, 85, 170, 205},	  // 3
	{103, 138, 170, 205}, // 4
	{155, 190, 170, 205}, // 5
	{50, 85, 215, 250},	  // 6
	{103, 138, 215, 250}, // 7
	{155, 190, 215, 250}, // 8
	{52, 263, 82, 292},	  // 9
	{103, 138, 260, 295}, // x
	{156, 263, 189, 296}, // v
	{105, 300, 30, 15},	  // point
	{7, 35, 8, 31},		  // icon return
};

/* check touch keypad MQTT */
key_MQTT_character_t check_event_keypad_MQTT(int16_t x, int16_t y)
{
	key_MQTT_character_t i;
	for (i = 0; i < 14; i++)
	{
		if (x > keypad_MQTT[i][0] && x < keypad_MQTT[i][1] && y > keypad_MQTT[i][2] && keypad_MQTT[i][3])
			break;
	}
	return i;
}
