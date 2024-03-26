#ifndef MAIN_H
#define MAIN_H

/*********************
 *      INCLUDES
 *********************/

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"

#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_wifi.h"

#include "wifi.h"
#include "gpio.h"
#include "nvs_rw.h" 
#include "uart_driver.h"
#include "mqtt_client_tcp.h"
#include "http_client_request.h"

/*********************
 *      DEFINES
 *********************/

#define MAIN_TAG    "Main"

#define TXD_PIN     (GPIO_NUM_17)
#define RXD_PIN     (GPIO_NUM_16)
#define STACK_SIZE  1024

// EVENT UART RX BIT
#define CONNECT_WIFI_BIT    (1 << 0)
#define SSID_WIFI_BIT       (1 << 1)
#define PASSWORD_WIFI_BIT   (1 << 2)
#define NUMBER_OFF_SSID     (1 << 3)
#define STATE_LED           (1 << 4)
#define AUTO_LED            (1 << 5)

// EVENT UART TX BIT

#define SSID_WIFI_SCAN_BIT      (1 << 0)
#define CONTROL_LED_MQTT_BIT    (1 << 1)

/**********************
 *      TYPEDEFS
 **********************/

typedef enum
{
    HEADING_DUMMY = 0X00,
    HEADING_SSID,
    HEADING_PASSWORD,
    HEADING_CONNECT_WIFI,
    HEADING_NUMBER_OFF_SSID,
    HEADING_STATE_LED,
    HEADING_ILLUMINANCE,
    HEADING_AUTO_LED,
} heading_data_t;

typedef enum
{
    STATE_ON_WIFI,
    STATE_OFF_WIFI,
} wifi_state_t;

/*********************
 *  EXTERN VARIABLE
 *********************/

extern TaskHandle_t mqttControlData_task;
extern TaskHandle_t connectWifi_task;
extern TaskHandle_t uart_rx_task;
extern TaskHandle_t uart_tx_task;
extern EventGroupHandle_t event_uart_rx_heading;

/*********************
 * ENTRY TASK FUNCTION
 *********************/

extern void startConnectWifiTask(void *arg);
extern void startUartTxTask(void *arg);
extern void startUartRxTask(void *arg);
extern void startMQTTControlDataTask(void *arg);

/*********************
 *   INLINE FUNCTION
 *********************/

static inline void initMain(void)
{
    NVS_Init();
    WIFI_Sta_Init();
    uartDriverInit(UART_NUM_1, TXD_PIN, RXD_PIN, 
                    115200, UART_DATA_8_BITS,
                    UART_PARITY_DISABLE, UART_HW_FLOWCTRL_DISABLE, 
                    UART_STOP_BITS_1);
    // mqtt_app_start(&mqtt_client_0, url_mqtt);
}

static inline void createMainTask(void)
{
    xTaskCreate(startUartTxTask, 
                "uart_tx_task", 
                STACK_SIZE * 2, 
                NULL, 
                8, 
                &uart_tx_task);

    xTaskCreate(startUartRxTask, 
                "uart_rx_task", 
                STACK_SIZE * 3, 
                NULL, 
                9, 
                &uart_rx_task);

    xTaskCreate(startConnectWifiTask, 
                "Wifi connect", 
                STACK_SIZE * 3, 
                NULL, 
                5, 
                &connectWifi_task);
    xTaskCreate(startMQTTControlDataTask, 
                "Wifi connect", 
                STACK_SIZE * 3, 
                NULL, 
                4, 
                &mqttControlData_task);
 
}

static inline void checkHeadingData(heading_data_t heading)
{
    switch (heading)
    {
    case HEADING_CONNECT_WIFI:
        xEventGroupSetBits(event_uart_rx_heading, CONNECT_WIFI_BIT);
        break;
    case HEADING_DUMMY:
        ESP_LOGI(MAIN_TAG, "HEADING DUMMY");
        break;
    default:
        ESP_LOGE(MAIN_TAG, "HEADING ERROR");
        break;
    }
}

/*********************
 *   STATIC FUNCTION
 *********************/

static void get_ssid_scan(uint8_t *ssid, uint8_t *memory, uint8_t index)
{
    uint8_t i = 0;
    while (i <= 32)
    {
        *(ssid + i) = *(memory + (32 * index) + i);
        if (*(ssid + i) == '\0')
            break;  
        i++;
    }
    
}
#endif /* MAIN_H */