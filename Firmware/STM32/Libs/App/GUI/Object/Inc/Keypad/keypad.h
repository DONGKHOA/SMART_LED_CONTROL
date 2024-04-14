#ifndef APP_GUI_OBJECT_KEYPAD_H_
#define APP_GUI_OBJECT_KEYPAD_H_

#include <stdint.h>
#include "Icon/icon.h"
#include "graphics.h"

typedef enum {
	NUM_0 = 0,
	NUM_1,
	NUM_2,
	NUM_3,
	NUM_4,
	NUM_5,
	NUM_6,
	NUM_7,
	NUM_8,
	NUM_9,
	NUM_x,
	NUM_v,
	NO_TOUCH,
}key_character_t;

__attribute__((always_inline)) inline void object_Keypad(void)
{
	GraphicsRoundedRectangle(50, 125, 35, 35, 5, BLACK);
	GraphicsRoundedRectangle(103, 125, 35, 35, 5, BLACK);
	GraphicsRoundedRectangle(155, 125, 35, 35, 5, BLACK);
	GraphicsRoundedRectangle(50, 170, 35, 35, 5, BLACK);
	GraphicsRoundedRectangle(103, 170, 35, 35, 5, BLACK);
	GraphicsRoundedRectangle(155, 170, 35, 35, 5, BLACK);
	GraphicsRoundedRectangle(50, 215, 35, 35, 5, BLACK);
	GraphicsRoundedRectangle(103, 215, 35, 35, 5, BLACK);
	GraphicsRoundedRectangle(155, 215, 35, 35, 5, BLACK);
	GraphicsRoundedRectangle(103, 260, 35, 35, 5, BLACK);

	GraphicsLargeCharacter(63, 134, '1', WHITE);
	GraphicsLargeCharacter(116, 134, '2', WHITE);
	GraphicsLargeCharacter(169, 134, '3', WHITE);
	GraphicsLargeCharacter(63, 179, '4', WHITE);
	GraphicsLargeCharacter(116, 179, '5', WHITE);
	GraphicsLargeCharacter(169, 179, '6', WHITE);
	GraphicsLargeCharacter(63, 225, '7', WHITE);
	GraphicsLargeCharacter(116, 225, '8', WHITE);
	GraphicsLargeCharacter(169, 225, '9', WHITE);
	GraphicsColourBitmap(52, 263, 30, 29, icon_delete);
	GraphicsLargeCharacter(116, 269, '0', WHITE);
	GraphicsColourBitmap(156, 263, 33, 33, icon_check);
}

key_character_t check_event_keypad(int16_t x, int16_t y);
#endif
