#include "main.h"


static uint8_t volatile ssid[32];
static uint8_t volatile pass[32];

uint8_t data_uart[1024];

void test(void *arg)
{
    while(1)
    {
        printf("%d\n",WIFI_Scan(data_uart));
        printf("%s\n", data_uart);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    ESP_ERROR_CHECK(NVS_Init());
    WIFI_StaInit();
    xTaskCreate(test, "DHT11", 1024 * 10, NULL, 10, NULL);
}