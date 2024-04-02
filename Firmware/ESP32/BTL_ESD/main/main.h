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

// IO UART
#define TXD_PIN     (GPIO_NUM_17)
#define RXD_PIN     (GPIO_NUM_16)

#define STACK_SIZE  1024

// UART RX EVENT 
#define ON_WIFI_BIT                             (1 << 0)
#define OFF_WIFI_BIT                            (1 << 1)
#define CONNECT_WIFI_RX_BIT                     (1 << 2)
#define CONNECT_WIFI_SCAN_BIT                   (1 << 3)
#define CONNECT_MQTT_BIT                        (1 << 4)

// UART TX EVENT 
#define SEND_NUMBER_WIFI_SCAN_BIT               (1 << 0)
#define SEND_CONNECT_WIFI_SUCCESSFUL_BIT        (1 << 1)
#define SEND_CONNECT_WIFI_UNSUCCESSFUL_BIT      (1 << 2)
#define SEND_SSID_CONNECT_WIFI_SUCCESSFUL_BIT   (1 << 3)
#define REFUSE_CONNECT_MQTT_BIT                 (1 << 4)
#define SEND_CONNECT_MQTT_SUCCESSFUL_BIT        (1 << 5)
#define SEND_CONNECT_MQTT_UNSUCCESSFUL_BIT      (1 << 6)
#define MQTT_PUBLISH_BIT                        (1 << 7)
#define MQTT_SUBSCRIBE_BIT                      (1 << 8)

/**********************
 *      TYPEDEFS
 **********************/

typedef enum 
{
    HEADING_ON_WIFI = 0x01,
    HEADING_OFF_WIFI,
    HEADING_CONNECT_WIFI,
    HEADING_CONNECT_MQTT,
    HEADING_AUTO_LED,
} uart_rx_heading_t;

typedef enum
{
    HEADING_SEND_NUMBER_WIFI_SCAN = 0x01,
    HEADING_SEND_NAME_WIFI_SCAN,
    HEADING_SEND_CONNECT_WIFI_SUCCESSFUL,
    HEADING_SEND_CONNECT_WIFI_UNSUCCESSFUL,
    HEADING_REFUSE_CONNECT_MQTT,
    HEADING_SEND_CONNECT_MQTT_SUCCESSFUL,
    HEADING_SEND_CONNECT_MQTT_UNSUCCESSFUL,
    HEADING_MQTT_PUBLISH,
    HEADING_MQTT_SUBSCRIBE,
} uart_tx_heading_t;

typedef enum
{
    CONNECTED_WIFI,
    DISCONNECTED_WIFI,
    CONNECTED_MQTT,
    DISCONNECTED_MQTT,
} state_connect_network_t;

/*********************
 *  EXTERN VARIABLE
 *********************/

extern TaskHandle_t wifiScan_task;
extern TaskHandle_t wifiConnect_task;
extern TaskHandle_t mqttConnect_task;
extern TaskHandle_t mqttControlData_task;
extern TaskHandle_t uart_rx_task;
extern TaskHandle_t uart_tx_task;

extern EventGroupHandle_t event_uart_rx_heading;
extern EventGroupHandle_t event_uart_tx_heading;

/*********************
 * ENTRY TASK FUNCTION
 *********************/

extern void startWifiScan(void *arg);
extern void startWifiConnectTask(void *arg);
extern void startMQTTConnectTask(void * arg);
extern void startMQTTControlDataTask(void *arg);
extern void startUartTxTask(void *arg);
extern void startUartRxTask(void *arg);

/*********************
 *   INLINE FUNCTION
 *********************/

static inline void initMain(void)
{
    NVS_Init();
    WIFI_StaInit();
    uartDriverInit(UART_NUM_1, TXD_PIN, RXD_PIN, 
                    115200, UART_DATA_8_BITS,
                    UART_PARITY_DISABLE, UART_HW_FLOWCTRL_DISABLE, 
                    UART_STOP_BITS_1);
    // mqtt_app_start(&mqtt_client_0, url_mqtt);
}

static inline void initServices(void)
{
    event_uart_rx_heading = xEventGroupCreate();
    event_uart_tx_heading = xEventGroupCreate();
}

static inline void createMainTask(void)
{
    xTaskCreate(startUartRxTask, 
                "uart_rx_task", 
                STACK_SIZE * 3, 
                NULL, 
                9, 
                &uart_rx_task);

    xTaskCreate(startUartTxTask, 
                "uart_tx_task", 
                STACK_SIZE * 2, 
                NULL, 
                8, 
                &uart_tx_task);

    xTaskCreate(startWifiScan, 
                "Wifi scan", 
                STACK_SIZE * 3, 
                NULL, 
                7, 
                &wifiScan_task);

    xTaskCreate(startWifiConnectTask, 
                "Wifi connect", 
                STACK_SIZE * 3, 
                NULL, 
                6, 
                &wifiConnect_task);

    xTaskCreate(startMQTTConnectTask, 
                "Mqtt connect", 
                STACK_SIZE * 3, 
                NULL, 
                5, 
                &mqttConnect_task);

    xTaskCreate(startMQTTControlDataTask, 
                "Mqtt control", 
                STACK_SIZE * 3, 
                NULL, 
                4, 
                &mqttControlData_task);
 
}

static inline void checkingHeadingRxData(uint8_t heading)
{
    switch (heading)
    {
    case HEADING_OFF_WIFI:
        xEventGroupClearBits(event_uart_rx_heading, ON_WIFI_BIT);
        xEventGroupSetBits(event_uart_rx_heading, OFF_WIFI_BIT);
        break;

    case HEADING_ON_WIFI:
        xEventGroupClearBits(event_uart_rx_heading, OFF_WIFI_BIT);
        xEventGroupSetBits(event_uart_rx_heading, ON_WIFI_BIT);
        break;

    case HEADING_CONNECT_WIFI:
        xEventGroupSetBits(event_uart_rx_heading, CONNECT_WIFI_RX_BIT);
        break;
    case HEADING_CONNECT_MQTT:
        xEventGroupSetBits(event_uart_rx_heading, CONNECT_MQTT_BIT);
        break;
    }
}

static inline void getSSID_PASS(uint8_t * data, uint8_t *ssid, uint8_t * pass)
{
    uint8_t i = 0;
    uint8_t position = 0;

    for (; *(data + i) != '\r'; i++)
    {
        *(ssid + position) = *(data + i);
        position++;
    }

    *(ssid + position) = '\0';
    position = 0;

    for (; *(data + i) != '\n'; i++)
    {
        *(pass + position) = *(data + i);
        position++;
    }
    *(pass + position) = '\0';
}

static inline int8_t matchingWIFIScan(uint8_t * data, uint8_t * ssid, uint8_t *pass)
{
    uint16_t i = 1;
    uint8_t position;
    uint8_t data_temp[32];
    
    while (*(data + i - 1) != '\0')
    {
        position = 1;
        data_temp[position - 1] = *(data + i - 1);
        while ((*(data + i) != '\r') && (*(data + i) != '\0'))
        {
            data_temp[position] = *(data + i);
            i++;
            position++;
        }

        i = i + 2;
        data_temp[position] = '\0';

        if (WIFI_ScanNVS(data_temp, pass) != -1)
        {
            memcpy(ssid, data_temp, strlen((char *)data_temp) + 1);
            return 1;
        }
    }
    return -1;
}

#endif /* MAIN_H */