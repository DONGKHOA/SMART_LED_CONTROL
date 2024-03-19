/*
 * screen_3.c
 *
 *  Created on: Mar 17, 2024
 *      Author: dongkhoa
 */
#include "screen.h"
#include "Keypad/keypad.h"

void screen_3(void)
{
	GraphicsClear(WHITE);
	GraphicsLargeString(35, 45, "ENTER PASSWORD", BLACK);
	GraphicsRoundedRectangle(35, 82, 170, 30, 5, BLACK);
	GraphicsColourBitmap(7, 8, 28, 23, icon_return);

	object_Keypad();
}
