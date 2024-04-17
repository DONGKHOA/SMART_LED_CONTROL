#include "Icon/icon.h"
#include "stdint.h"
#include "check_touch_screen/check_touch_screen.h"
// same src "check_touch_screen_1"
// have icon return, and list wifi current
static const uint16_t frame_pos[6][4] = {   // frame position wifi
    {7, 35, 8, 31}, // icon return
    // thieu 1 cai frame cua wifi dang ket noi
    {34, 204, 117, 142},    
    { 34, 204, 156, 181},
    {34, 204, 195, 220},
    {34, 204, 234, 259},
    {34, 204, 273, 298},
};
touch_icon_screen2_t check_event_icon_screen2(int16_t x, int16_t y)
{
    touch_icon_screen2_t i;
    for(i = 0; i < 6; i++)
    {
        if(x > frame_pos[i][0] && x < frame_pos[i][1] && y > frame_pos[i][2] && y < frame_pos[i][3] )
        break;
    }
    return i;
}
