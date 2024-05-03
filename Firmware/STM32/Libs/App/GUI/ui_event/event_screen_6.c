#include "event.h"
#include "check_touch_screen/check_touch_screen.h"
#include "screen.h"
#include "main.h"
#include "FreeRTOS.h"
#include "event_groups.h"

EventBits_t uxBits;

void check_event_screen_6(screen_state_t *screen)
{
    if ( uxBits & DETECT_TOUCH_SCREEN_BIT)
    {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 0);    //On backlight

        bit_map_screen_1.screen = 1;        // return screen_1 if touched_screen
        bit_map_screen_1.wifi = 1;
        bit_map_screen_1.home = 1;
        bit_map_screen_1.MQTT = 1;
        bit_map_screen_1.state_connected_MQTT = 1;
        bit_map_screen_1.state_connected_WIFI = 1;
        *screen = SCREEN_START;
    }
}
