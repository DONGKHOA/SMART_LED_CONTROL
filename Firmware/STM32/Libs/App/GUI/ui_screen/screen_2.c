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
uint8_t numPage = 1;
uint8_t limitNumPage;
uint8_t u8_quantitySSID = 0;
char ssid1[32];
char ssid2[32];
char ssid3[32];
char ssid4[32];
char ssid5[32];
char ssid_connect[32];

/**********************
 *  EXTERN VARIABLES
 **********************/

extern uint8_t buffer_uart_rx[RX_BUFFER_SIZE + 1];
extern EventGroupHandle_t event_uart_rx;
extern EventGroupHandle_t event_uart_tx;
extern TimerHandle_t timer_request_scan_wifi;

/**********************
 *  STATIC VARIABLES
 **********************/

static char dataSSID[65];

/******************************
 *  STATIC PROTOTYPE FUNCTION
 ******************************/

static void processingSSID(char *data, uint8_t page);
static uint8_t getNumPage(uint8_t quantitySSID);

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
		GraphicsRoundedRectangle(34, 54, 170, 25, 5, WHITE);

	if (uxBits & NUMBER_WIFI_SCAN_BIT)
	{
		u8_quantitySSID = atoi((const char *)buffer_uart_rx);
		limitNumPage = getNumPage(u8_quantitySSID);
		if (numPage > limitNumPage)
			numPage = limitNumPage;
	}

	if (uxBits & NAME_WIFI_SCAN_BIT)
	{
		memcpy(dataSSID, buffer_uart_rx, strlen((char *)buffer_uart_rx) + 1);
	}

	if (uxBits & CONNECT_WIFI_SUCCESSFUL_BIT)
	{
		bit_map_screen_2.WIFI_Connected = 1;
		GraphicsRoundedRectangle(34, 54, 170, 25, 5, BLACK);
		GraphicsLargeString(40, 57, ssid_connect, WHITE); // in ssid connected
	}

	if (state_wifi == 1)
	{
		processingSSID(dataSSID, numPage);

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
			if (numPage == limitNumPage)
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
		xTimerReset(timer_request_scan_wifi, 0);
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

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * The function `processingSSID` processes and stores SSID data into arrays based on the page number
 * and sets corresponding bit fields.
 *
 * @param data The `processingSSID` function takes a `char* data` and a `uint8_t page` as parameters.
 * The `data` parameter is a pointer to a character array containing information about SSIDs, and the
 * `page` parameter is an integer representing the current page number.
 * @param page The `page` parameter in the `processingSSID` function is used to determine which page of
 * SSIDs to process. It is an unsigned 8-bit integer (`uint8_t`) that indicates the current page number
 * being processed. This parameter helps in calculating the position of SSIDs in the input data
 */
static void processingSSID(char *data, uint8_t page)
{
	uint8_t numSSID = 5;
	char *arg_list[4];
	char buffer[1024];
	memcpy(buffer, data, strlen((char *)data));
	uint8_t arg_position = 0;

	// cut string
	char *temp_token = strtok(buffer, "\r");
	while (temp_token != NULL)
	{
		arg_list[arg_position] = temp_token;
		arg_position++;
		temp_token = strtok(NULL, "\r");
	}

	// Store data in array and set bit filed

	if ((page == limitNumPage) && ((u8_quantitySSID % SSID_IN_PAGE) > 0))
	{
		numSSID = u8_quantitySSID % SSID_IN_PAGE;
	}

	switch (numSSID)
	{
	case 0:
	case 5:
		memcpy(ssid5, arg_list[4 + (page - 1) * 5], strlen((char *)arg_list[4 + (page - 1) * 5]) + 1);
		bit_map_screen_2.WIFI5 = 1;
	case 4:
		memcpy(ssid4, arg_list[3 + (page - 1) * 5], strlen((char *)arg_list[3 + (page - 1) * 5]) + 1);
		bit_map_screen_2.WIFI4 = 1;
	case 3:
		memcpy(ssid3, arg_list[2 + (page - 1) * 5], strlen((char *)arg_list[2 + (page - 1) * 5]) + 1);
		bit_map_screen_2.WIFI3 = 1;
	case 2:
		memcpy(ssid2, arg_list[1 + (page - 1) * 5], strlen((char *)arg_list[1 + (page - 1) * 5]) + 1);
		bit_map_screen_2.WIFI2 = 1;
	case 1:
		memcpy(ssid1, arg_list[0 + (page - 1) * 5], strlen((char *)arg_list[0 + (page - 1) * 5]) + 1);
		bit_map_screen_2.WIFI1 = 1;
		break;
	}
}

/**
 * The function calculates the number of pages needed based on the quantity of SSIDs.
 *
 * @param quantitySSID The parameter `quantitySSID` represents the quantity of SSIDs (Service Set
 * Identifiers) that you have. The function `getNumPage` calculates the number of pages required to
 * display all the SSIDs, with a maximum of 5 SSIDs per page.
 *
 * @return The function `getNumPage` returns the number of pages needed to display a given quantity of
 * SSIDs, based on the calculation logic provided in the code snippet.
 */
static uint8_t getNumPage(uint8_t quantitySSID)
{
	uint8_t quantity = 0;
	if (quantitySSID == 0)
	{
		return 0;
	}
	else
	{
		quantity = quantitySSID / SSID_IN_PAGE;
		if ((quantitySSID % SSID_IN_PAGE) > 0)
			quantity++;
		return quantity;
	}
}
