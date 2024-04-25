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
#include "freertos/timers.h"
#include "freertos/queue.h"
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
#define TXD_PIN                                 (GPIO_NUM_17)
#define RXD_PIN                                 (GPIO_NUM_16)

#define MIN_STACK_SIZE                          1024

#define TIME_MQTT_SUBSCRIBE                     2000
#define MQTT_PUBLISH                            0
#define MQTT_SUBSCRIBE                          1

// UART RX EVENT 
#define ON_WIFI_BIT                             (1 << 0)
#define OFF_WIFI_BIT                            (1 << 1)
#define CONNECT_WIFI_RX_BIT                     (1 << 2)
#define CONNECT_WIFI_SCAN_BIT                   (1 << 3)
#define CONNECT_MQTT_BIT                        (1 << 4)

// UART TX EVENT 
#define SEND_NUMBER_NAME_WIFI_SCAN_BIT          (1 << 0)
#define SEND_CONNECT_WIFI_SUCCESSFUL_BIT        (1 << 1)
#define SEND_CONNECT_WIFI_UNSUCCESSFUL_BIT      (1 << 2)
#define REFUSE_CONNECT_MQTT_BIT                 (1 << 3)
#define SEND_CONNECT_MQTT_SUCCESSFUL_BIT        (1 << 4)
#define SEND_CONNECT_MQTT_UNSUCCESSFUL_BIT      (1 << 5)

/**********************
 *      TYPEDEFS
 **********************/

typedef enum 
{
    HEADING_WIFI = 0x01,
    HEADING_CONNECT_WIFI,
    HEADING_CONNECT_MQTT,
    HEADING_MQTT_PUBLISH,
} uart_rx_heading_t;

typedef enum
{
    HEADING_SEND_NUMBER_WIFI_SCAN = 0x01,
    HEADING_SEND_NAME_WIFI_SCAN,
    HEADING_SEND_CONNECT_WIFI,
    HEADING_SEND_CONNECT_MQTT,
    HEADING_MQTT_SUBSCRIBE,
} uart_tx_heading_t;

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

static inline int8_t matchingWIFIScan(char * data, uint8_t * ssid, uint8_t *pass)
{
    char *arg_list[50];
    char buffer[1024];
    memcpy(buffer, data, strlen((char *) data));
    uint8_t arg_position = 0;

    // cut string
    char *temp_token = strtok(buffer, "\r");
    while(temp_token != NULL)
    {
        arg_list[arg_position]= temp_token;
        arg_position++;
        temp_token = strtok(NULL, "\r");
    }

    // check matching ssid in NVS
    for (uint8_t i = 0; i < arg_position; i++)
    {
        if (WIFI_ScanNVS((uint8_t *)arg_list[i], pass) != -1)
        {
            memcpy(ssid, arg_list[i], strlen((char *)arg_list[i]) + 1);
            return 1;
        }
    }
    return -1;
}

static inline void processingDataMQTTPublish(char * data, char *state_led, 
                char * state_auto, char *lux, char * temperature)
{
    char *arg_list[4];
    char buffer[1024];
    memcpy(buffer, data, strlen((char *) data));
    uint8_t arg_position = 0;

    // cut string
    
    char *temp_token = strtok(buffer, "\r");
    while(temp_token != NULL)
    {
        arg_list[arg_position]= temp_token;
        arg_position++;
        temp_token = strtok(NULL, "\r");
    }

    // Store data in array

    memcpy(state_led, arg_list[0], strlen((char *)arg_list[0]) + 1);
    memcpy(state_auto, arg_list[1], strlen((char *)arg_list[1]) + 1);
    memcpy(lux, arg_list[2], strlen((char *)arg_list[2]) + 1);
    memcpy(temperature, arg_list[3], strlen((char *)arg_list[3]) + 1);
}

/*********************
 *   GLOBAL FUNCTION
 *********************/

void transmissionFrameData(uart_tx_heading_t heading, char *data)
{
    uartSendData(UART_NUM_2, (char *)&heading);
    uartSendData(UART_NUM_2, data);
    uartSendData(UART_NUM_2, "\n");
    
}
#endif /* MAIN_H */