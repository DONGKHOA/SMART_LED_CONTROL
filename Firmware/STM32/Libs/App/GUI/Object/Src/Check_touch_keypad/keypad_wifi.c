/*********************
 *      INCLUDES
 *********************/

#include "Keypad/keypad_wifi.h"

/* The `static const uint16_t keypad_wifi[13][4]` array in the code snippet is defining a 2D array that
represents the coordinates of keys on a WiFi keypad interface. Each row in the array corresponds to
a specific key on the keypad, and each key is defined by four values representing the x and y
coordinates for the top-left and bottom-right corners of the key's bounding box. */
static const uint16_t keypad_wifi[13][4] = {
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
	{7, 35, 8, 31},		  // icon return
};

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * The function `check_event_keypad_Wifi` checks for a key press event on a WiFi keypad based on the
 * coordinates x and y.
 * 
 * @param x The parameter `x` represents the x-coordinate of a touch event on a keypad interface for
 * WiFi settings.
 * @param y The parameter `y` represents the y-coordinate of a touch event on a keypad interface for a
 * WiFi module. The function `check_event_keypad_Wifi` is designed to check which key on the keypad has
 * been pressed based on the x and y coordinates of the touch event.
 * 
 * @return The function `check_event_keypad_Wifi` is returning a variable of type `key_character_t`,
 * which represents the index of the keypad_wifi array that satisfies the conditions specified in the
 * if statement within the for loop. If no such index is found, the function will return the value of
 * `i` after the loop has completed, which would be 13 in this case.
 */
key_character_t check_event_keypad_Wifi(int16_t x, int16_t y)
{
	key_character_t i;
	for (i = 0; i < 13; i++)
	{
		if (x > keypad_wifi[i][0] && x < keypad_wifi[i][1] && y > keypad_wifi[i][2] && keypad_wifi[i][3])
			return i;
	}
	return i;
}