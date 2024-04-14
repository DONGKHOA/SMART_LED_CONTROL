/*
 * screen_1.c
 *
 *  Created on: Mar 17, 2024
 *      Author: dongkhoa
 */
#include "screen.h"
#include "Icon/icon.h"
#include "graphics.h"

void screen_1(void)
{
	GraphicsClear(WHITE);
	GraphicsColourBitmap(99, 75, 41, 39, icon_wifi);
	GraphicsColourBitmap(95, 154, 50, 48, icon_home);
	GraphicsColourBitmap(7, 8, 28, 23, icon_return);
}
