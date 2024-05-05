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
#define MQTT_PUBLISH_BIT                        (1 << 5)

// UART TX EVENT 
#define SEND_NUMBER_NAME_WIFI_SCAN_BIT          (1 << 0)
#define SEND_CONNECT_WIFI_SUCCESSFUL_BIT        (1 << 1)
#define SEND_CONNECT_WIFI_UNSUCCESSFUL_BIT      (1 << 2)
#define SEND_SSID_CONNECT_WIFI_SUCCESSFUL_BIT   (1 << 3)
#define SEND_REFUSE_CONNECT_MQTT_BIT                 (1 << 4)
#define SEND_CONNECT_MQTT_SUCCESSFUL_BIT        (1 << 5)
#define SEND_CONNECT_MQTT_UNSUCCESSFUL_BIT      (1 << 6)
#define MQTT_SUBSCRIBE_BIT                      (1 << 7)

/**********************
 *      TYPEDEFS
 **********************/

typedef enum 
{
    HEADING_ON_WIFI = 0x01,
    HEADING_OFF_WIFI,
    HEADING_CONNECT_WIFI,
    HEADING_CONNECT_MQTT,
    HEADING_MQTT_PUBLISH,
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
 *   INLINE FUNCTION
 *********************/

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

/*********************
 *   GLOBAL FUNCTION
 *********************/

void transmissionFrameData(uart_tx_heading_t heading, char *data)
{
    if (heading == HEADING_SEND_NAME_WIFI_SCAN)
    {
        uartSendData(UART_NUM_1, (char *)&heading);
        uartSendData(UART_NUM_1, &data);
        uartSendData(UART_NUM_1, "\n");
    }
    else
    {
        uartSendData(UART_NUM_1, (char *)&heading);
        uartSendData(UART_NUM_1, "\n");
    }
    
}
#endif /* MAIN_H */