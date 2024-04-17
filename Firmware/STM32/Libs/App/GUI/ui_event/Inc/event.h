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

typedef enum
{
	EVENT_SCREEN_1 = 0,
	EVENT_SCREEN_2,
	EVENT_SCREEN_3,
	EVENT_SCREEN_4
} __attribute__((packed)) check_event_t;

extern QueueHandle_t queue_data_tx;
extern QueueHandle_t queue_data_rx;

void check_event_screen_1(check_event_t *event,
						  screen_state_t *screen);
void check_event_screen_2(check_event_t *event,
						  screen_state_t *screen);
void check_event_screen_3(check_event_t *event,
						  screen_state_t *screen);
void check_event_screen_4(check_event_t *event,
						  screen_state_t *screen);

#endif /* APP_GUI_UI_EVENT_INC_EVENT_H_ */
