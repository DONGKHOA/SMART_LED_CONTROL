/*
 * event.h
 *
 *  Created on: Apr 14, 2024
 *      Author: Author: vuhuy
 */

#ifndef APP_GUI_UI_EVENT_INC_EVENT_H_
#define APP_GUI_UI_EVENT_INC_EVENT_H_

#include "screen.h"
#include "FreeRTOS.h"
#include "queue.h"

extern QueueHandle_t queue_data_tx;
extern QueueHandle_t queue_data_rx;

void check_event_screen_1(screen_state_t *screen);
void check_event_screen_2(screen_state_t *screen);
void check_event_screen_3(screen_state_t *screen);
void check_event_screen_4(screen_state_t *screen);
void check_event_screen_5(screen_state_t *screen);

#endif /* APP_GUI_UI_EVENT_INC_EVENT_H_ */
