/*
 * screen.h
 *
 *  Created on: Apr 14, 2024
 *      Author: ACER
 */

#ifndef APP_GUI_UI_SCREEN_INC_SCREEN_H_
#define APP_GUI_UI_SCREEN_INC_SCREEN_H_

#include <stdio.h>

typedef enum
{
	SCREEN_START = 0,
	SCREEN_WIFI,
	SCREEN_KEYPAD,
	SCREEN_MAIN
} screen_state_t;

void screen_1(void);
void screen_2(void);
void screen_3(void);
void screen_4(void);

#endif /* APP_GUI_UI_SCREEN_INC_SCREEN_H_ */
