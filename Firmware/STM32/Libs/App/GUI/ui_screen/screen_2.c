/*********************
 *      INCLUDES
 *********************/

#include "screen.h"
#include "Icon/icon.h"
#include "graphics.h"
#include "main.h"
#include <string.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "event_groups.h"

/**********************
 *     VARIABLES
 **********************/

field_bit_screen2_t bit_map_screen_2;
uint8_t state_wifi = 0;
uint8_t numPage = 1;
uint8_t limitNumPage;
uint8_t u8_quantitySSID = 0;
char ssid1[16];
char ssid2[16];
char ssid3[16];
char ssid4[16];
char ssid5[16];

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

static uint8_t connected = 0;
static char dataSSID[65];

/******************************
 *  STATIC PROTOTYPE FUNCTION
 ******************************/

static void processingSSID(char *data, uint8_t offset);
static uint8_t getNumPage(uint8_t quantitySSID);

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void functionRequestCallBack( TimerHandle_t xTimer )
{
	xEventGroupSetBits(event_uart_tx, ON_WIFI_BIT);
}

/**
 * The function `screen_2` handles various graphical elements and events related to Wi-Fi connectivity
 * on a screen.
 */
void screen_2(void)
{
	if (bit_map_screen_2.screen == 1)
	{
		GraphicsClear(WHITE);
		GraphicsRoundedRectangle(34, 54, 170, 25, 5, BLACK);
		bit_map_screen_2.screen = 0;
	}

	if (bit_map_screen_2.ret == 1)
	{
		GraphicsColourBitmap(5, 7, 28, 23, icon_return);
		bit_map_screen_2.ret = 0;
	}

	if (bit_map_screen_2.text1 == 1)
	{
		GraphicsLargeString(80, 25, "Wi-Fi", BLACK);
		bit_map_screen_2.text1 = 0;
	}

	if (bit_map_screen_2.text2 == 1)
	{
		GraphicsLargeString(5, 92, "Wi-Fi Network", BLACK);
		GraphicsHline(0, 240, 90, BLACK);
		bit_map_screen_2.text2 = 0;
	}

	if (bit_map_screen_2.on_off_wifi == 1)
	{
		if (state_wifi == 1)
		{
			GraphicsRoundedRectangle(185, 10, 20, 20, 3, GREEN);
		}
		else
		{
			GraphicsRoundedRectangle(185, 10, 20, 20, 3, RED);
		}
	}

	EventBits_t uxBits = xEventGroupWaitBits(event_uart_rx, NUMBER_WIFI_SCAN_BIT | NAME_WIFI_SCAN_BIT | CONNECT_WIFI_SUCCESSFUL_BIT | DETECT_TOUCH_SCREEN_BIT,
											 pdTRUE, pdFALSE,
											 portMAX_DELAY);
	if (uxBits & NUMBER_WIFI_SCAN_BIT)
	{
		u8_quantitySSID = atoi((const char *)buffer_uart_rx);
		limitNumPage = getNumPage(u8_quantitySSID);
	}

	if (uxBits & NAME_WIFI_SCAN_BIT)
	{
		memcpy(dataSSID, buffer_uart_rx, strlen((char *)buffer_uart_rx) + 1);
	}

	if (uxBits & CONNECT_WIFI_SUCCESSFUL_BIT)
	{
		if (bit_map_screen_2.WIFI_Connected == 1)
		{
			GraphicsRoundedRectangle(34, 54, 170, 25, 5, BLACK);
			// in ssid connected
			connected = 1;
			bit_map_screen_2.WIFI_Connected = 0;
		}
	}

	if (uxBits & DETECT_TOUCH_SCREEN_BIT)
	{
		if (state_wifi == 1)
		{
			processingSSID(dataSSID, numPage);

			if (bit_map_screen_2.WIFI1 == 1)
			{
				// in ssid 1
				GraphicsRoundedRectangle(34, 117, 170, 25, 5, BLACK);
				bit_map_screen_2.WIFI1 = 0;
			}

			if (bit_map_screen_2.WIFI2 == 1)
			{
				// in ssid 2
				GraphicsRoundedRectangle(34, 156, 170, 25, 5, BLACK);
				bit_map_screen_2.WIFI2 = 0;
			}

			if (bit_map_screen_2.WIFI3 == 1)
			{
				// in ssid 3
				GraphicsRoundedRectangle(34, 195, 170, 25, 5, BLACK);
				bit_map_screen_2.WIFI3 = 0;
			}

			if (bit_map_screen_2.WIFI4 == 1)
			{
				// in ssid 4
				GraphicsRoundedRectangle(34, 234, 170, 25, 5, BLACK);
				bit_map_screen_2.WIFI4 = 0;
			}

			if (bit_map_screen_2.WIFI5 == 1)
			{
				// in ssid 5
				GraphicsRoundedRectangle(34, 273, 170, 25, 5, BLACK);
				bit_map_screen_2.WIFI5 = 0;
			}

			if (bit_map_screen_2.NEXT == 1)
			{
				if (numPage == limitNumPage)
				{
					// write character with WHITE
				}
				else
				{
					// write character with BLACK
				}

				bit_map_screen_2.NEXT = 1;
			}

			if (bit_map_screen_2.BACK == 1)
			{
				if (numPage == 1)
				{
					// write character with WHITE
				}
				else
				{
					// write character with BLACK
				}

				bit_map_screen_2.BACK = 1;
			}
			xTimerReset(timer_request_scan_wifi, 0);
		}
		else
		{
			xTimerStop(timer_request_scan_wifi, 0);
			connected = 0;
			GraphicsRoundedRectangle(34, 54, 170, 25, 5, WHITE);
			GraphicsRoundedRectangle(34, 117, 170, 25, 5, WHITE);
			GraphicsRoundedRectangle(34, 156, 170, 25, 5, WHITE);
			GraphicsRoundedRectangle(34, 195, 170, 25, 5, WHITE);
			GraphicsRoundedRectangle(34, 234, 170, 25, 5, WHITE);
			GraphicsRoundedRectangle(34, 273, 170, 25, 5, WHITE);
			// add icon on off wifi
		}
	}
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * The function `processingSSID` processes a string input, extracts substrings separated by "\r", and
 * stores them in different variables.
 *
 * @param data The `processingSSID` function takes a character array `data` and an offset value as
 * input parameters. The `data` array contains a list of SSIDs (Service Set Identifiers) separated by
 * carriage return characters. The function processes this data and extracts individual SSIDs based on
 * the offset provided.
 * @param offset The `offset` parameter in the `processingSSID` function is used to determine which set
 * of SSIDs to process from the input data. It is used to calculate the starting position for
 * extracting SSID information from the `arg_list` array. The formula `(offset - 1) * 5
 */
static void processingSSID(char *data, uint8_t offset)
{
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

	// Store data in array
	memcpy(ssid1, arg_list[0 + (offset - 1) * 5], strlen((char *)arg_list[0 + (offset - 1) * 5]) + 1);
	memcpy(ssid2, arg_list[1 + (offset - 1) * 5], strlen((char *)arg_list[1 + (offset - 1) * 5]) + 1);
	memcpy(ssid3, arg_list[2 + (offset - 1) * 5], strlen((char *)arg_list[2 + (offset - 1) * 5]) + 1);
	memcpy(ssid4, arg_list[3 + (offset - 1) * 5], strlen((char *)arg_list[3 + (offset - 1) * 5]) + 1);
	memcpy(ssid5, arg_list[4 + (offset - 1) * 5], strlen((char *)arg_list[4 + offset * 5]) + 1);
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
		quantity = quantitySSID / 5;
		if ((quantitySSID % 5) > 0)
			quantity++;
		return quantity;
	}
}