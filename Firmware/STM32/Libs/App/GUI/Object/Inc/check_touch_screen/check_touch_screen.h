/*
 * check_touch_screen.h
 *
 *  Created on: Apr 15, 2024
 *      Author: vuhuy
 */

#ifndef APP_GUI_OBJECT_INC_CHECK_TOUCH_SCREEN_CHECK_TOUCH_SCREEN_H_
#define APP_GUI_OBJECT_INC_CHECK_TOUCH_SCREEN_CHECK_TOUCH_SCREEN_H_

#include "stdint.h"

typedef enum
{
    NO_TOUCH_ICON_SC1 = -1,
    ICON_WIFI = 0,
    ICON_HOME,
    ICON_MQTT,
} touch_icon_screen1_t;

typedef enum
{
    NO_TOUCH_ICON_SC2 = -1,
    ICON_RETURN = 0,
    WIFI1,
    WIFI2,
    WIFI3,
    WIFI4,
    WIFI5,
} touch_icon_screen2_t;

touch_icon_screen1_t check_event_icon_screen1(int16_t x, int16_t y);
touch_icon_screen2_t check_event_icon_screen2(int16_t x, int16_t y);

#endif /* APP_GUI_OBJECT_INC_CHECK_TOUCH_SCREEN_CHECK_TOUCH_SCREEN_H_ */
