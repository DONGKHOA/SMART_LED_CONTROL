#include "main.h"

uint8_t data_uart[1024];
uint8_t count = 182;

static uint8_t 			ssid[32] = "HUY";
static uint8_t 			pass[32] = "Huy211202";
static char			url_mqtt[30] = "mqtt://192.168.137.77:1883";
MQTT_Client_Data_t mqtt_client_0;
SemaphoreHandle_t mqtt_semaphore;

static void startUartTxTask(void *arg)
{
    while (1)
    {
        // if (xSemaphoreTake(mqtt_semaphore, 500) == pdTRUE)
        // {
        //     count++;
        // }
        
        printf("%d\n", count);
    }
}


void app_main(void)
{
    NVS_Init();
    WIFI_StaInit();
    WIFI_Connect(ssid, pass);
        sprintf(url_mqtt, "mqtt://192.168.137.%d:1883", count);
        // transmissionFrameData(HEADING_SEND_NUMBER_WIFI_SCAN, NULL);
        MQTT_app_start(&mqtt_client_0, url_mqtt);
    mqtt_semaphore = xSemaphoreCreateBinary();

    xTaskCreate(startUartTxTask, 
                "uart_tx_task", 
                STACK_SIZE * 5, 
                NULL, 
                2, 
                NULL);
}