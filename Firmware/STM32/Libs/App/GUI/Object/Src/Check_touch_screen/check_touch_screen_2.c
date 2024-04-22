#include "Icon/icon.h"
#include "stdint.h"
#include "check_touch_screen/check_touch_screen.h"

/* position of each icon screen_2 */
static const uint16_t frame_pos[7][4] = {
    {7, 35, 8, 31},      // icon return
    {34, 204, 54, 80},   // frame wifi connected
    {34, 204, 117, 142}, // frame wifi 1
    {34, 204, 156, 181}, // frame wifi 2
    {34, 204, 195, 220}, // frame wifi 3
    {34, 204, 234, 259}, // frame wifi 4
    {34, 204, 273, 298}, // frame wifi 5
};

/* check touch icon */
touch_icon_screen2_t check_event_icon_screen2(int16_t x, int16_t y)
{
    touch_icon_screen2_t i;
    for (i = 0; i < NO_TOUCH_ICON_SC2; i++)
    {
        if (x > frame_pos[i][0] && x < frame_pos[i][1] && y > frame_pos[i][2] && y < frame_pos[i][3])
            break;
    }
    return i;
}
