/*
 * screen_3.c
 *
 *  Created on: Mar 17, 2024
 *      Author: thanh
 */
#include "../screen.h"
#include "icon.h"
void screen_3(void)
{
	GraphicsClear(WHITE);
	GraphicsLargeString(35, 45, "ENTER PASSWORD", BLACK);
	GraphicsRoundedRectangle(35, 82, 170, 30, 5, BLACK);
	GraphicsColourBitmap(7, 8, 28, 23, icon_return);

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
