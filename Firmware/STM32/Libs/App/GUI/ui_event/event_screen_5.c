#include "event.h"
#include "Keypad/keypad_MQTT.h"

extern int16_t x;
extern int16_t y;

static const char character_key[13] = {'0', '1', '2', '3',
									   '4', '5', '6', '7',
									   '8', '9', 'x', 'v', '.'};

static uint8_t MQTT[12];
static uint8_t MQTT_pos = 0;
uint8_t connect_MQTT = 0;

void check_event_screen_5(check_event_t *event,
                            screen_state_t *screen)
{
    static uint16_t x_coordinate = 0; 

    key_MQTT_character_t  key  = check_event_keypad_MQTT(x, y);
    if(key != NO_TOUCH)
    {
        if(key == NUM_x)
        {
            for (MQTT_pos = 0; MQTT_pos < 12; MQTT_pos++)
            {
                MQTT[MQTT_pos] = 0;
            }
            bit_map_screen_5.frame = 1;
            MQTT_pos = 0;
            *event = EVENT_SCREEN_5;
            x_coordinate = 0;
        }
    }
}         

