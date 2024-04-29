#include "event.h"
#include "check_touch_screen/check_touch_screen.h"
#include "screen.h"
#include "main.h"
#include "FreeRTOS.h"
#include "event_groups.h"

extern EventGroupHandle_t event_uart_rx;
extern TimerHandle_t timer_wait_off_screen;
EventBits_t uxBits;

void check_event_screen_6(screen_state_t *screen){
if ( uxBits & DETECT_TOUCH_SCREEN_BIT)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 0);    //On backlight

    bit_map_screen_1.screen = 1;        // return screen_1 if touched_screen
    bit_map_screen_1.wifi = 1;
    bit_map_screen_1.home = 1;
    bit_map_screen_1.MQTT = 1;
    bit_map_screen_1.connected_MQTT = 1;
    bit_map_screen_1.connected_WIFI = 1;
    *screen = SCREEN_START;

    xTimerReset( timer_wait_off_screen, 10 );   // reset Timer
}
}
