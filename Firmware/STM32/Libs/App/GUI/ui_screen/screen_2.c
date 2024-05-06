/*********************
 *      INCLUDES
 *********************/

#include "screen.h"
#include "Icon/icon.h"
#include "graphics.h"
#include "main.h"
#include <string.h>
#include <stdlib.h>

/*********************
 *      DEFINES
 *********************/

#define MAX_LENGTH_SSID 13
#define SSID_IN_PAGE 5


/**********************
 *     VARIABLES
 **********************/

field_bit_screen2_t bit_map_screen_2;
uint8_t state_wifi = 0;
uint8_t numSSIDofPage = 0;

char ssid1[32];
char ssid2[32];
char ssid3[32];
char ssid4[32];
char ssid5[32];

char ssid_connect[32];

/**********************
 *  STATIC VARIABLES
 **********************/

char bufferEffectScreen2[174];

/**********************
 *  EXTERN VARIABLES
 **********************/

extern uint8_t buffer_uart_rx[RX_BUFFER_SIZE + 1];
extern EventGroupHandle_t event_uart_rx;
extern EventGroupHandle_t event_uart_tx;
extern TimerHandle_t timer_request_scan_wifi;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * The function `screen_2` handles various graphical elements and logic related to Wi-Fi connectivity
 * on a screen.
 *
 * @param uxBits The `uxBits` parameter in the `screen_2` function is a bitmask representing various
 * event bits that have been set. The function checks these bits to determine which actions to take
 * based on the events that have occurred. Each bit in the `uxBits` parameter corresponds to a specific
 * event or
 */
void screen_2(EventBits_t uxBits)
{
	if (uxBits & SCAN_WIFI_BIT)
	{
		char *token;
		uint8_t position = 0;
		strcpy(bufferEffectScreen2, buffer_uart_rx);

		token = strtok(bufferEffectScreen2, "\r");
		while (token != NULL) 
		{
			if ()
			{
				
			}
			
			token = strtok(NULL, "\r");
		}
	}
	
	if (bit_map_screen_2.screen == 1)
	{
		GraphicsClear(WHITE);
		GraphicsRoundedRectangle(35, 54, 170, 25, 5, BLACK);
		bit_map_screen_2.screen = 0;
	}

	if (bit_map_screen_2.ret == 1)
	{
		GraphicsColourBitmap(5, 7, 28, 23, icon_return);
		bit_map_screen_2.ret = 0;
	}

	if (bit_map_screen_2.text1 == 1)
	{
		GraphicsLargeString(165, 34, "Wi-Fi", BLACK);
		bit_map_screen_2.text1 = 0;
	}

	if (bit_map_screen_2.text2 == 1)
	{
		GraphicsLargeString(5, 90, "Wi-Fi Network", BLACK);
		GraphicsHline(0, 240, 90, BLACK);
		bit_map_screen_2.text2 = 0;
	}

	if (bit_map_screen_2.on_off_wifi == 1)
	{
		if (state_wifi == 1)
		{
			GraphicsRoundedRectangle(174, 8, 22, 22, 3, GREEN); // green if wifi on
		}
		else
		{
			GraphicsRoundedRectangle(174, 8, 22, 22, 3, RED); // red if wifi off
		}
	}

	if (bit_map_screen_2.WIFI_Connected == 1)
	{
		GraphicsRoundedRectangle(34, 54, 170, 25, 5, BLACK);
		GraphicsLargeString(40, 57, ssid_connect, WHITE); // in ssid connected
	}
	else
		GraphicsRoundedRectangle(34, 54, 170, 35, 5, WHITE);

	if (uxBits & CONNECT_WIFI_SUCCESSFUL_BIT)
	{
		bit_map_screen_2.WIFI_Connected = 1;
		GraphicsRoundedRectangle(34, 54, 170, 25, 5, BLACK);
		GraphicsLargeString(40, 57, ssid_connect, WHITE); // in ssid connected
	}

	if (state_wifi == 1)
	{
		if (bit_map_screen_2.WIFI1 == 1)
		{
			GraphicsRoundedRectangle(35, 117, 170, 25, 5, BLACK);
			GraphicsLargeString(40, 120, ssid1, WHITE); // in ssid 1
			bit_map_screen_2.WIFI1 = 0;
		}
		else
			GraphicsRoundedRectangle(35, 117, 170, 25, 5, WHITE);

		if (bit_map_screen_2.WIFI2 == 1)
		{
			GraphicsRoundedRectangle(35, 156, 170, 25, 5, BLACK);
			GraphicsLargeString(40, 160, ssid2, WHITE); // in ssid 2
			bit_map_screen_2.WIFI2 = 0;
		}
		else
			GraphicsRoundedRectangle(34, 156, 170, 25, 5, WHITE);

		if (bit_map_screen_2.WIFI3 == 1)
		{
			GraphicsRoundedRectangle(35, 195, 170, 25, 5, BLACK);
			GraphicsLargeString(40, 200, ssid3, WHITE); // in ssid 3
			bit_map_screen_2.WIFI3 = 0;
		}
		else
			GraphicsRoundedRectangle(35, 195, 170, 25, 5, WHITE);

		if (bit_map_screen_2.WIFI4 == 1)
		{
			GraphicsRoundedRectangle(35, 234, 170, 25, 5, BLACK);
			GraphicsLargeString(40, 240, ssid4, WHITE); // in ssid 4
			bit_map_screen_2.WIFI4 = 0;
		}
		else
			GraphicsRoundedRectangle(35, 234, 170, 25, 5, WHITE);

		if (bit_map_screen_2.WIFI5 == 1)
		{
			GraphicsRoundedRectangle(35, 273, 170, 25, 5, BLACK);
			GraphicsLargeString(40, 280, ssid5, WHITE); // in ssid 5
			bit_map_screen_2.WIFI5 = 0;
		}
		else
			GraphicsRoundedRectangle(35, 273, 170, 25, 5, WHITE);

		if (bit_map_screen_2.NEXT == 1)
		{
			if (numPage == 1)
			{
				GraphicsLargeString(182, 298, "NEXT->", WHITE); // write character with WHITE
			}
			else
			{
				GraphicsLargeString(182, 298, "NEXT->", BLACK); // write character with BLACK
			}

			bit_map_screen_2.NEXT = 1;
		}

		if (bit_map_screen_2.BACK == 1)
		{
			if (numPage == 1)
			{
				GraphicsLargeString(11, 298, "<-BACK", WHITE); // write character with WHITE
			}
			else
			{
				GraphicsLargeString(11, 298, "<-BACK", BLACK); // write character with BLACK
			}

			bit_map_screen_2.BACK = 1;
		}
	}
	else
	{
		xTimerStop(timer_request_scan_wifi, 0);
		bit_map_screen_2.WIFI_Connected = 0;
		GraphicsRoundedRectangle(34, 54, 170, 25, 5, WHITE);
		GraphicsRoundedRectangle(34, 117, 170, 25, 5, WHITE);
		GraphicsRoundedRectangle(34, 156, 170, 25, 5, WHITE);
		GraphicsRoundedRectangle(34, 195, 170, 25, 5, WHITE);
		GraphicsRoundedRectangle(34, 234, 170, 25, 5, WHITE);
		GraphicsRoundedRectangle(34, 273, 170, 25, 5, WHITE);
	}
}