#include "main.h"

// Variable Handle

static int16_t data_uart_position = 0;
static uint8_t data[32];
static heading_data_t flag = HEADING_DUMMY;

static uint8_t 			ssid[32] = "BK21";
static uint8_t 			pass[32] = "18072003";
static uint8_t is_init = 0;

// Task Handle

TaskHandle_t connectWifi_task;
TaskHandle_t uart_rx_task;
TaskHandle_t uart_tx_task;

// Queue Handle

static QueueHandle_t queue_uart_data;

// EventGroup Handle

EventGroupHandle_t event_uart_heading;

void startConnectWifiTask(void *arg)
{
    while (1)
    {
        WIFI_Scan();
        if (is_init == 0)
        {
            if (WIFI_Connect(ssid, pass) == CONNECT_OK)
            {
                is_init = 1;
            }
        }
        vTaskDelay(2 / portTICK_PERIOD_MS);
    }
    
}

void startUartTxTask(void *arg)
{
    while (1) 
    {
        vTaskDelay(2 / portTICK_PERIOD_MS);
    }
}


void startUartRxTask(void *arg)
{
    uint8_t buf_temp[RX_BUF_SIZE + 1];

    while (1) 
    {
        uint8_t rxBytes = uart_read_bytes(UART_NUM_1, &buf_temp, 
                            RX_BUF_SIZE, 10 / portTICK_PERIOD_MS);
        
        while (rxBytes >= 0)
        {
            if (data_uart_position == -1)
            {
                flag = buf_temp[rxBytes];
                rxBytes--;
                data_uart_position++;
                continue;
            }
            
            data[data_uart_position] = buf_temp[rxBytes];
            rxBytes--;
            data_uart_position++;
        }
        
        if (data[data_uart_position] == '\n')
        {
            data[data_uart_position] = '\0';
            data_uart_position = 0;
            checkHeadingData(flag);
            xQueueSend(queue_uart_data, (void *)&data, 0);
        }
        
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    initMain();
    queue_uart_data = xQueueCreate(RX_BUF_SIZE + 1, sizeof(uint8_t));
    data_uart_position = -1;
    event_uart_heading = xEventGroupCreate();
    createMainTask();
    
}