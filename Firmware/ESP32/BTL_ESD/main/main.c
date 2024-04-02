#include "main.h"

// Variable Handle

static int16_t volatile data_uart_rx_pos = 0;
static uint8_t volatile data_uart[RX_BUF_SIZE + 1];
static int8_t volatile data_heading = -1;

static uint8_t volatile ssid[32];
static uint8_t volatile pass[32];

static state_connect_network_t volatile flag_connected_wifi = 0;

static uint8_t num_wifi_scan = 0;

static char url_mqtt[30] = "mqtt://172.16.0.181:1883";

// Task Handle

TaskHandle_t wifiScan_task;
TaskHandle_t wifiConnect_task;
TaskHandle_t mqttConnect_task;
TaskHandle_t mqttControlData_task;
TaskHandle_t uart_rx_task;
TaskHandle_t uart_tx_task;

// EventGroup Handle

EventGroupHandle_t event_uart_rx_heading;
EventGroupHandle_t event_uart_tx_heading;

MQTT_Client_Data_t mqtt_client_0;

void startWifiScan(void *arg)
{
    while (1)
    {
        EventBits_t uxBits = xEventGroupWaitBits(event_uart_rx_heading,
                                                 ON_WIFI_BIT | 
                                                 OFF_WIFI_BIT,
                                                 pdFALSE, pdFALSE, 
                                                 portMAX_DELAY);

        if (uxBits & ON_WIFI_BIT)
        {
            /* 
                - Store all ssid and pass scan to array data_uart.
                - Check ssid & pass 
                    - Matching ssid & pass in nvs -> 
                    set bit CONNECT_WIFI_SCAN_BIT.
                    - Don't matching ssid & pass in nvs ->
                    store ssid & pass in nvs.
            */
            num_wifi_scan = WIFI_Scan((uint8_t *)data_uart);
            if (matchingWIFIScan((uint8_t *)data_uart, (uint8_t *)ssid, (uint8_t *)pass) != -1)
            {
                xEventGroupSetBits(event_uart_rx_heading, 
                                    CONNECT_WIFI_SCAN_BIT);
            }
            
            xEventGroupSetBits(event_uart_tx_heading, 
                                    SEND_NUMBER_WIFI_SCAN_BIT);
        }

        if (uxBits & OFF_WIFI_BIT)
        {
            esp_wifi_stop();
        }
        
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void startWifiConnectTask(void *arg)
{
    while (1)
    {
        EventBits_t uxBits = xEventGroupWaitBits(event_uart_rx_heading,
                                                CONNECT_WIFI_RX_BIT |
                                                CONNECT_WIFI_SCAN_BIT,
                                                pdTRUE, pdFALSE, 
                                                portMAX_DELAY);

        if (uxBits & CONNECT_WIFI_RX_BIT)
        {
            /*
                - Connect successful -> set bit SEND_CONNECT_WIFI_SUCCESSFUL
                                    -> store ssid & pass in nvs
                - Connect unsuccessful -> set bit SEND_CONNECT_WIFI_UNSUCCESSFUL
            */
            getSSID_PASS((uint8_t *)data_uart, (uint8_t *)ssid, (uint8_t *)pass);
            if (WIFI_Connect((uint8_t *)ssid, (uint8_t *)pass) == CONNECT_OK)
            {
                WIFI_StoreNVS((uint8_t *)ssid, (uint8_t *)pass);
                xEventGroupSetBits(event_uart_tx_heading, 
                                        SEND_CONNECT_WIFI_SUCCESSFUL_BIT);
            }
            else
            {
                xEventGroupSetBits(event_uart_tx_heading, 
                                        SEND_CONNECT_WIFI_UNSUCCESSFUL_BIT);
            }
        }

        if (uxBits & CONNECT_WIFI_SCAN_BIT)
        {
            if (WIFI_Connect((uint8_t *)ssid, (uint8_t *)pass) == CONNECT_OK)
            {
                xEventGroupSetBits(event_uart_tx_heading, 
                                        SEND_CONNECT_WIFI_SUCCESSFUL_BIT);
            }
            else
            {
                xEventGroupSetBits(event_uart_tx_heading, 
                                        SEND_CONNECT_WIFI_UNSUCCESSFUL_BIT);
            }
        }
    }
}

void startMQTTConnectTask(void * arg)
{
    while (1)
    {
        EventBits_t uxBits = xEventGroupWaitBits(event_uart_rx_heading,
                                                CONNECT_MQTT_BIT,
                                                pdTRUE, pdFALSE, 
                                                portMAX_DELAY);
        if (uxBits & CONNECT_MQTT_BIT)
        {
            /*
                If the system is connected to wifi, mqtt connection is allowed
                If the system is not connected to wifi, mqtt connection is not allowed
            */
            if (flag_connected_wifi == 1)
            {
                
            }
            else
            {
                xEventGroupSetBits(event_uart_tx_heading, 
                                    REFUSE_CONNECT_MQTT_BIT);
            }   
        }   
    }
}

void startMQTTControlDataTask(void *arg)
{
    while (1)
    {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void startUartTxTask(void *arg)
{
    while (1)
    {
        EventBits_t uxBits = xEventGroupWaitBits(event_uart_rx_heading,
                                                SEND_NUMBER_WIFI_SCAN_BIT | 
                                                SEND_CONNECT_WIFI_SUCCESSFUL_BIT |
                                                SEND_CONNECT_WIFI_UNSUCCESSFUL_BIT |
                                                REFUSE_CONNECT_MQTT_BIT |
                                                SEND_CONNECT_MQTT_SUCCESSFUL_BIT |
                                                SEND_CONNECT_MQTT_UNSUCCESSFUL_BIT,
                                                pdTRUE, pdFALSE, 
                                                portMAX_DELAY);
        switch (uxBits)
        {
            case SEND_NUMBER_WIFI_SCAN_BIT:
                uint8_t buffer_temp[32];

                memcpy(buffer_temp, (uint8_t *)data_uart, strlen((char *)data_uart) + 1);

                sprintf((char *)data_uart, "%d%d\n", HEADING_SEND_NUMBER_WIFI_SCAN, 
                                            num_wifi_scan);
                uartSendData(UART_NUM_1, data_uart);
                
                sprintf((char *)data_uart, "%d%s\n", HEADING_SEND_NAME_WIFI_SCAN, 
                                            buffer_temp);
                uartSendData(UART_NUM_1, data_uart);
                break;

            case SEND_CONNECT_WIFI_SUCCESSFUL_BIT:
                flag_connected_wifi = 1;
                sprintf((char *)data_uart, "%d\n", HEADING_SEND_CONNECT_WIFI_SUCCESSFUL);
                uartSendData(UART_NUM_1, data_uart);
                break;

            case SEND_SSID_CONNECT_WIFI_SUCCESSFUL_BIT:
                flag_connected_wifi = 0;
                sprintf((char *)data_uart, "%d\n", HEADING_SEND_CONNECT_WIFI_UNSUCCESSFUL);
                uartSendData(UART_NUM_1, data_uart);
                break;
            case REFUSE_CONNECT_MQTT_BIT:
                sprintf((char *)data_uart, "%d\n", HEADING_REFUSE_CONNECT_MQTT);
                uartSendData(UART_NUM_1, data_uart);
                break;
            
            case SEND_CONNECT_MQTT_SUCCESSFUL_BIT:
                sprintf((char *)data_uart, "%d\n", HEADING_SEND_CONNECT_MQTT_SUCCESSFUL);
                uartSendData(UART_NUM_1, data_uart);
                break;

            case SEND_CONNECT_MQTT_UNSUCCESSFUL_BIT:
                sprintf((char *)data_uart, "%d\n", HEADING_SEND_CONNECT_MQTT_UNSUCCESSFUL);
                uartSendData(UART_NUM_1, data_uart);
                break;
        }
    }
}

void startUartRxTask(void *arg)
{
    uint8_t buffer_temp[RX_BUF_SIZE + 1];

    while (1)
    {
        uint16_t rxBytes = uart_read_bytes(UART_NUM_1, &buffer_temp,
                                           RX_BUF_SIZE, 10 / portTICK_PERIOD_MS);

        data_heading = buffer_temp[0];
        data_uart_rx_pos = rxBytes;
        
        for (uint16_t i = 1; i < rxBytes; i++)
        {   
            data_uart[i] = buffer_temp[i];
        }

        if (data_uart[data_uart_rx_pos] == '\n')
        {
            data_uart[data_uart_rx_pos] = '\0';
        }

        checkingHeadingRxData(data_heading);

        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}


void app_main(void)
{
    printf ("Main task start\n");
    initMain();
    initServices();
    createMainTask();
    printf ("Main task end\n");
}