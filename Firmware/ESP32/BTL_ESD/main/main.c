#include "main.h"

// Variable Handle

static int16_t data_uart_rx_position;
static uint8_t volatile data_uart_rx[32];
static uint8_t volatile data_uart_tx[RX_BUF_SIZE + 1];

static uint8_t volatile flag_connect_wifi;
static heading_data_t volatile flag = HEADING_DUMMY;

int8_t volatile count = 0;

static uint8_t 			ssid[32] = "KOPHiN COFFEE";
static uint8_t 			pass[32] = "kophincamon";
// static uint8_t is_init = 0;

MQTT_Client_Data_t mqtt_client_0;
// static char url_mqtt[30] = "mqtt://172.16.0.181:1883";

// Task Handle

TaskHandle_t connectWifi_task;
TaskHandle_t mqttControlData_task;
TaskHandle_t uart_rx_task;
TaskHandle_t uart_tx_task;

// Queue Handle

static QueueHandle_t queue_uart_rx_data;

// EventGroup Handle

EventGroupHandle_t event_uart_rx_heading;
EventGroupHandle_t event_uart_tx_heading;

// Semaphore Handle


void startConnectWifiTask(void *arg)
{
    uint8_t data_buff[RX_BUF_SIZE + 1];
    while (1)
    {
        EventBits_t uxBits = xEventGroupWaitBits(event_uart_rx_heading,
                                                CONNECT_WIFI_BIT |
                                                SSID_WIFI_BIT |
                                                PASSWORD_WIFI_BIT,
                                                pdFALSE, pdFALSE,
                                                portMAX_DELAY);
        if (uxBits & CONNECT_WIFI_BIT)
        {
            if (xQueueReceive(queue_uart_rx_data, &data_buff, portMAX_DELAY))
            {
                if (data_buff[0] == STATE_OFF_WIFI)
                {
                    esp_wifi_stop();
                }
                else if (data_buff[0] == STATE_ON_WIFI)
                {
                    count = WIFI_Scan();
                    xEventGroupSetBits(event_uart_rx_heading, SSID_WIFI_SCAN_BIT);
                }
            }
            xEventGroupClearBits(event_uart_rx_heading, CONNECT_WIFI_BIT);
        }

        if (uxBits & SSID_WIFI_BIT)
        {
            if (xQueueReceive(queue_uart_rx_data, &ssid, portMAX_DELAY))
            {
                flag_connect_wifi++;
            }
            xEventGroupClearBits(event_uart_rx_heading, SSID_WIFI_BIT);
        }

        if (uxBits & PASSWORD_WIFI_BIT)
        {
            if (xQueueReceive(queue_uart_rx_data, &pass, portMAX_DELAY))
            {
                flag_connect_wifi++;
            }
            xEventGroupClearBits(event_uart_rx_heading, PASSWORD_WIFI_BIT);
        }

        if (flag_connect_wifi == 2)
        {
            WIFI_Connect(ssid, pass);
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void startUartTxTask(void *arg)
{
    while (1)
    {
        EventBits_t uxBits = xEventGroupWaitBits(event_uart_rx_heading,
                                                SSID_WIFI_SCAN_BIT |
                                                CONTROL_LED_MQTT_BIT,
                                                pdTRUE, pdFALSE,
                                                portMAX_DELAY);

        if (uxBits & SSID_WIFI_SCAN_BIT)
        {
            sprintf((char *)data_uart_tx, "%d%d\n", HEADING_NUMBER_OFF_SSID,
                    count);

            uartSendData(UART_NUM_1, data_uart_tx);
            memset((void *)data_uart_tx, 0, sizeof(data_uart_tx));

            for (uint8_t i = 0; i < count; i++)
            {
                char ssid[32];
                get_ssid_scan((uint8_t *)ssid, ssid_name, i);
                sprintf((char *)data_uart_tx, "%d%s\n", HEADING_SSID,
                        ssid);

                uartSendData(UART_NUM_1, data_uart_tx);
                memset((void *)data_uart_tx, 0, sizeof(data_uart_tx));
            }
        }
        if (uxBits & CONTROL_LED_MQTT_BIT)
        {
            
        }
        
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void startUartRxTask(void *arg)
{
    uint8_t buf_temp[RX_BUF_SIZE + 1];

    while (1)
    {
        uint16_t rxBytes = uart_read_bytes(UART_NUM_1, &buf_temp,
                                           RX_BUF_SIZE, 10 / portTICK_PERIOD_MS);

        for (uint16_t i = 0; i < rxBytes; i++)
        {
            if (data_uart_rx_position == -1)
            {
                flag = buf_temp[rxBytes];
                data_uart_rx_position++;
                continue;
            }

            data_uart_rx[data_uart_rx_position] = buf_temp[rxBytes];
            data_uart_rx_position++;
        }

        if (data_uart_rx[data_uart_rx_position] == '\n')
        {
            data_uart_rx[data_uart_rx_position] = '\0';
            checkHeadingData(flag);
            xQueueSend(queue_uart_rx_data, (void *)&data_uart_rx, 0);
            memset((void *)data_uart_rx, 0, sizeof(data_uart_rx));
            data_uart_rx_position = -1;
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void startMQTTControlDataTask(void *arg)
{
    while (1)
    {
        esp_mqtt_client_publish(mqtt_client_0.client, "test", "123", 0, 1, 0);
        esp_mqtt_client_subscribe(mqtt_client_0.client, "test_1", 0);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
    
}

void app_main(void)
{
    printf("main start\n");
    initMain();
    // WIFI_Connect(ssid, pass);
    queue_uart_rx_data = xQueueCreate(RX_BUF_SIZE + 1, sizeof(uint8_t));
    event_uart_rx_heading = xEventGroupCreate();
    event_uart_tx_heading = xEventGroupCreate();
    data_uart_rx_position = -1;
    // createMainTask();
    printf("main end\n");
    // vTaskSuspend(connectWifi_task);
}