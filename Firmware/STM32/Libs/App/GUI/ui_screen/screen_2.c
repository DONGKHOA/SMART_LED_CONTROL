/*
 * screen_2.c
 *
 *  Created on: Mar 17, 2024
 *      Author: dongkhoa
 */
#include "screen.h"
#include "Icon/icon.h"
#include "graphics.h"

void screen_2(void)
{
	GraphicsColourBitmap(5, 7, 28, 23, icon_return);
	GraphicsLargeString(71, 25, "Wi-Fi", BLACK);
	GraphicsRoundedRectangle(34, 54, 170, 25, 5, BLACK);
	GraphicsHline(0, 240, 90, BLACK);
	GraphicsLargeString(5, 92, "Wi-Fi Network", BLACK);

	GraphicsRoundedRectangle(34, 117, 170, 25, 5, BLACK);
	GraphicsRoundedRectangle(34, 156, 170, 25, 5, BLACK);
	GraphicsRoundedRectangle(34, 195, 170, 25, 5, BLACK);
	GraphicsRoundedRectangle(34, 234, 170, 25, 5, BLACK);
	GraphicsRoundedRectangle(34, 273, 170, 25, 5, BLACK);
}
