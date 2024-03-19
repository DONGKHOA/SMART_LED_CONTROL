#include "main.h"

static TaskHandle_t connectWifi_task;

static uint8_t is_init = 0;

// static uint8_t 			ssid[32] = "Khoa";
// static uint8_t 			pass[32] = "17042021";

static uint8_t 			ssid[32] = "TheCupsCoffee";
static uint8_t 			pass[32] = "Thecups2022";

// static uint8_t 			ssid[32] = "KEYBOX KAFE";
// static uint8_t 			pass[32] = "xincamon";

void startConnectWifiTask(void *arg)
{
    WIFI_Sta_Init();
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

void app_main(void)
{
    NVS_Init();
    xTaskCreate(startConnectWifiTask, 
            "Wifi connect", 
            1024 * 3, 
            NULL, 
            10, 
            &connectWifi_task);
    
}

/* UART asynchronous example, that uses separate RX and TX tasks

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"

static const int RX_BUF_SIZE = 1024;

#define TXD_PIN (GPIO_NUM_4)
#define RXD_PIN (GPIO_NUM_5)

void init(void)
{
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    // We won't use a buffer for sending data.
    uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

static void rx_task(void *arg)
{
    static const char *RX_TASK_TAG = "RX_TASK";
    esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);
    uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE + 1);
    while (1) {
        const int rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, 1 / portTICK_PERIOD_MS);
        if (rxBytes > 0) {
            data[rxBytes] = 0;
            ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%s'", rxBytes, data);
            ESP_LOG_BUFFER_HEXDUMP(RX_TASK_TAG, data, rxBytes, ESP_LOG_INFO);
        }
    }
    free(data);
}

void app_main(void)
{
    init();
    xTaskCreate(rx_task, "uart_rx_task", 1024 * 2, NULL, configMAX_PRIORITIES - 1, NULL);
}

*/