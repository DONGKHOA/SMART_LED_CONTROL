/*
 * screen_4.c
 *
 *  Created on: Mar 17, 2024
 *      Author: thanh
 */
#include "../screen.h"
#include "icon.h"
void screen_4(void)
{
	GraphicsClear(WHITE);
	GraphicsColourBitmap(7, 8, 28, 23, icon_return);
	GraphicsLargeString(95, 119, "ON", BLACK);	 // if Button on
	GraphicsLargeString(126, 119, "OFF", BLACK); // if Button off
	GraphicsColourBitmap(91, 134, 59, 60, icon_button);
}
