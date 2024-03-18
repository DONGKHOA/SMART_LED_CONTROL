#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_err.h"

#include "wifi.h"

static uint8_t 			ssid[32] = "Khoa";
static uint8_t 			pass[32] = "17042021";

void app_main(void)
{
    WIFI_Scan();
    // WIFI_Station_Init(ssid, pass);
    while (1)
    {
        
    }
    
}
