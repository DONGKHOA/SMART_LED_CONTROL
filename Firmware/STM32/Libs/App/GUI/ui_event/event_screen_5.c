/*********************
 *      INCLUDES
 *********************/

#include "event.h"
#include "Keypad/keypad_MQTT.h"
#include "string.h"
#include "main.h"

/*********************
 *      DEFINES
 *********************/

#define OFFSET_X 15
#define OFFSET_Y 90
#define MIN_LENGTH_MQTT 15

/**********************
 *  STATIC VARIABLES
 **********************/

static const char character_key[13] = {'0', '1', '2', '3',
                                       '4', '5', '6', '7',
                                       '8', '9', 'x', 'v', '.'};

uint8_t MQTT[16];
uint8_t MQTT_pos = 0;

/**********************
 *  EXTERN VARIABLES
 **********************/

extern int16_t x;
extern int16_t y;

extern uint8_t buffer_uart_tx[RX_BUFFER_SIZE + 1];
extern EventGroupHandle_t event_uart_rx;
extern EventGroupHandle_t event_uart_tx;

extern char text_sc5[14];
extern EventBits_t bitsScreen5;

uint8_t state_connected_MQTT = 0;

/*MQTT screen are similar to Wifi screen in terms of functionality*/
void check_event_screen_5(screen_state_t *screen)
{
    static uint16_t x_coordinate = 0;

    key_MQTT_character_t key = check_event_keypad_MQTT(x, y);
    if (key != NO_TOUCH)
    {
        if (key == NUM_x)
        {
           memset(MQTT, '\0', sizeof(MQTT));
            bit_map_screen_5.frame = 1;
            MQTT_pos = 0;
            x_coordinate = 0;
        }

        else if (key == ICON_RETURN)
        {
            memset(MQTT, '\0', sizeof(MQTT));
            MQTT_pos = 0;
            x_coordinate = 0;

            bit_map_screen_1.screen = 1;
			bit_map_screen_1.wifi = 1;
			bit_map_screen_1.home = 1;
			bit_map_screen_1.MQTT = 1;
            *screen = SCREEN_START;
        }

        else if (key == NUM_v)
        {
            if(MQTT_pos >= (MIN_LENGTH_MQTT - 1))
            {
                MQTT_pos = 0;
                x_coordinate = 0;
                sprintf((char *)buffer_uart_tx, "%s", MQTT);
                xEventGroupSetBits(event_uart_tx, CONNECT_MQTT_BIT);
                memset(MQTT, '\0', sizeof(MQTT));
            }
           else
           {
            bit_map_screen_5.text = 1;
            strcpy(text_sc5, "error code");
           }
        }

        else
        {
            MQTT[MQTT_pos] = character_key[key];
            GraphicsLargeCharacter(x_coordinate, OFFSET_Y, character_key[key], WHITE);
            MQTT_pos++;
            x_coordinate += OFFSET_X;
        }
    }
    if(bitsScreen5 & CONNECT_MQTT_SUCCESSFUL_BIT)
    {
        bit_map_screen_5.MQTT_Connected = 1;
        bit_map_screen_1.state_connected_MQTT = 1;
        bit_map_screen_1.screen = 1;
        bit_map_screen_1.wifi = 1;
        bit_map_screen_1.home = 1;
        bit_map_screen_1.MQTT = 1;
        *screen = SCREEN_START;
    }
}