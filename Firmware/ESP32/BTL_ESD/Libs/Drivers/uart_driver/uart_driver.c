/*********************
 *      INCLUDES
 *********************/

#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "sdkconfig.h"

#include "uart_driver.h"

void uartDriverInit(uart_port_t uart_port, gpio_num_t tx_pin, gpio_num_t rx_pin,
                    uint32_t baudrate,
                    uart_word_length_t data_bit, 
                    uart_parity_t parity_bit, 
                    uart_hw_flowcontrol_t flow_control,
                    uart_stop_bits_t stop_bit)
{
    const uart_config_t uart_config = {
        .baud_rate = baudrate,
        .data_bits = data_bit,
        .parity = parity_bit,
        .stop_bits = stop_bit,
        .flow_ctrl = flow_control,
        .source_clk = UART_SCLK_DEFAULT,
    };

    // using buffer in rx data
    uart_driver_install(uart_port, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(uart_port, &uart_config);
    uart_set_pin(uart_port, tx_pin, rx_pin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

void uartSendData(uart_port_t uart_port, const char* data)
{
    const int len = strlen(data);
    uart_write_bytes(uart_port, data, len);
}