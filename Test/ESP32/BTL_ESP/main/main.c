#include "main.h"

uint8_t data_uart[1024];
static void startUartTxTask(void *arg)
{
    while (1)
    {
        transmissionFrameData(HEADING_SEND_NUMBER_WIFI_SCAN, NULL);
        printf("tx\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}


void app_main(void)
{
    uartDriverInit(UART_NUM_1, TXD_PIN, RXD_PIN, 
                    115200, UART_DATA_8_BITS,
                    UART_PARITY_DISABLE, UART_HW_FLOWCTRL_DISABLE, 
                    UART_STOP_BITS_1);

    xTaskCreate(startUartTxTask, 
                "uart_tx_task", 
                STACK_SIZE * 5, 
                NULL, 
                2, 
                NULL);
}