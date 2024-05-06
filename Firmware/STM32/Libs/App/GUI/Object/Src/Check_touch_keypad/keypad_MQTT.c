#include "Keypad/keypad_MQTT.h"

/* position of each element in the keyboard */
static const uint16_t keypad_MQTT[14][4] = {
		{50, 85, 125, 160},	  // 1
			{103, 138, 125, 160}, // 2
			{155, 190, 125, 160}, // 3
			{50, 85, 170, 205},	  // 4
			{103, 138, 170, 205}, // 5
			{155, 190, 170, 205}, // 6
			{50, 85, 215, 250},	  // 7
			{103, 138, 215, 250}, // 8
			{155, 190, 215, 250}, // 9
			{50, 85, 270, 295},	  // X
			{103, 138, 270, 295}, // 0
			{156, 190, 270, 295}, // v
			{105, 135, 300, 315},	  // point
			{7, 55, 8, 50},      // icon return
};

/* check touch keypad MQTT */
key_MQTT_character_t check_event_keypad_MQTT(int16_t x, int16_t y)
{
	key_MQTT_character_t i;
	for (i = 0; i < 14; i++)
	{
		if ((x > keypad_MQTT[i][0]) && (x < keypad_MQTT[i][1]) && (y > keypad_MQTT[i][2]) && (y < keypad_MQTT[i][3]))
		{
			return i;
		}
	}
	return i;
}
