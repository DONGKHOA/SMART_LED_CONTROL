#include "main.h"

static uint8_t 			ssid[32] = "BK21";
static uint8_t 			pass[32] = "18072003";
static uint8_t is_init = 0;

TaskHandle_t connectWifi_task;
TaskHandle_t uart_rx_task;
TaskHandle_t uart_tx_task;


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
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
    
}

void tx_task(void *arg)
{
    while (1) {
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}


void rx_task(void *arg)
{
    static const char *RX_TASK_TAG = "RX_TASK";
    esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);
    uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE + 1);
    while (1) {
        const int rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, 10 / portTICK_PERIOD_MS);
        if (rxBytes > 0) {
            data[rxBytes] = 0;
            ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%s'", rxBytes, data);
            ESP_LOG_BUFFER_HEXDUMP(RX_TASK_TAG, data, rxBytes, ESP_LOG_INFO);
        }
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
    free(data);
}

void app_main(void)
{
    NVS_Init();
    WIFI_Sta_Init();
    uartDriverInit(UART_NUM_1, TXD_PIN, RXD_PIN, 
                    115200, UART_DATA_8_BITS,
                    UART_PARITY_DISABLE, UART_HW_FLOWCTRL_DISABLE, 
                    UART_STOP_BITS_1);
    createMainTask();
}