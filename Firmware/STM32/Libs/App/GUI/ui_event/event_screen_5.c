/*********************
 *      INCLUDES
 *********************/

#include "event.h"
#include "Keypad/keypad_MQTT.h"

/*********************
 *      DEFINES
 *********************/

#define OFFSET_X 15
#define OFFSET_Y 90

/**********************
 *  STATIC VARIABLES
 **********************/

static const char character_key[13] = {'0', '1', '2', '3',
                                       '4', '5', '6', '7',
                                       '8', '9', 'x', 'v', '.'};

static uint8_t MQTT[15];
static uint8_t MQTT_pos = 0;

/**********************
 *  EXTERN VARIABLES
 **********************/

extern int16_t x;
extern int16_t y;

uint8_t connect_MQTT = 0;

/*MQTT screen are similar to Wifi screen in terms of functionality*/
void check_event_screen_5(screen_state_t *screen)
{
    static uint16_t x_coordinate = 0;

    key_MQTT_character_t key = check_event_keypad_MQTT(x, y);
    if (key != NO_TOUCH)
    {
        if (key == NUM_x)
        {
            for (MQTT_pos = 0; MQTT_pos < 15; MQTT_pos++)
            {
                MQTT[MQTT_pos] = 0;
            }
            bit_map_screen_5.frame = 1;
            MQTT_pos = 0;
            x_coordinate = 0;
        }

        else if (key == ICON_RETURN)
        {
            for (MQTT_pos = 0; MQTT_pos < 15; MQTT_pos++)
            {
                MQTT[MQTT_pos] = 0;
            }
            
            bit_map_screen_1.screen = 1;
			bit_map_screen_1.wifi = 1;
			bit_map_screen_1.home = 1;
			bit_map_screen_1.MQTT = 1;
            *screen = SCREEN_START;
        }

        else if (key == NUM_v)
        {
            MQTT_pos = 0;
            // xQueueSend(queue_data_tx, &MQTT, 0);
            // xQueueReceive(queue_data_rx, &temp, portMAX_DELAY);
            // check success or unsuccess, if success set *screen = screen_4
            x_coordinate = 0;
            for (MQTT_pos = 0; MQTT_pos < 15; MQTT_pos++)
            {
                MQTT[MQTT_pos] = 0;
            }
        }

        else
        {
            x_coordinate += OFFSET_X;
            MQTT[MQTT_pos] = character_key[key];
            MQTT_pos++;
            GraphicsLargeCharacter(x_coordinate, OFFSET_Y, character_key[key], WHITE);
        }
    }
}
