#ifndef APP_GUI_UI_SCREEN_INC_SCREEN_H_
#define APP_GUI_UI_SCREEN_INC_SCREEN_H_

/*********************
 *      INCLUDES
 *********************/

#include <stdio.h>
#include "stdint.h"

/**********************
 *      TYPEDEFS
 **********************/

typedef struct
{
	uint8_t screen : 1;
	uint8_t wifi : 1;
	uint8_t home : 1;
	uint8_t MQTT : 1;
} __attribute__((packed)) field_bit_screen1_t;

typedef struct
{
	uint8_t screen : 1;
	uint8_t ret : 1;
	uint8_t on_off_wifi : 1;
	uint8_t text1 : 1;
	uint8_t text2 : 1;
	uint8_t WIFI_Connected : 1;
	uint8_t WIFI1 : 1;
	uint8_t WIFI2 : 1;
	uint8_t WIFI3 : 1;
	uint8_t WIFI4 : 1;
	uint8_t WIFI5 : 1;
	uint8_t NEXT : 1;
	uint8_t BACK : 1;
} __attribute__((packed)) field_bit_screen2_t;

typedef struct
{
	uint8_t screen : 1;
	uint8_t ret : 1;
	uint8_t text : 1;
	uint8_t frame : 1;
	uint8_t key : 1;
} __attribute__((packed)) field_bit_screen3_t;

typedef struct
{
	uint8_t screen : 1;
	uint8_t ret : 1;
	uint8_t ON : 1;
	uint8_t OFF : 1;
	uint8_t control : 1;
	uint8_t automode : 1;
} __attribute__((packed)) field_bit_screen4_t;

typedef struct
{
	uint8_t screen : 1;
	uint8_t ret : 1;
	uint8_t text : 1;
	uint8_t frame : 1;
	uint8_t key : 1;
} __attribute__((packed)) field_bit_screen5_t;

typedef enum
{
	SCREEN_START = 0,
	SCREEN_WIFI,
	SCREEN_KEYPAD,
	SCREEN_MAIN,
	SCREEN_MQTT
} screen_state_t;

/**********************
 *  EXTERN VARIABLES
 **********************/

extern field_bit_screen1_t bit_map_screen_1;
extern field_bit_screen2_t bit_map_screen_2;
extern field_bit_screen3_t bit_map_screen_3;
extern field_bit_screen4_t bit_map_screen_4;
extern field_bit_screen5_t bit_map_screen_5;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void screen_1(void);
void screen_2(void);
void screen_3(void);
void screen_4(void);
void screen_5(void);

#endif /* APP_GUI_UI_SCREEN_INC_SCREEN_H_ */
