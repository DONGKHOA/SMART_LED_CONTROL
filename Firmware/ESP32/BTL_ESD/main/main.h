#ifndef MAIN_H
#define MAIN_H

/*********************
 *      INCLUDES
 *********************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_err.h"

#include "wifi.h"
#include "nvs_rw.h" 
#include "uart_driver.h"

/*********************
 *      DEFINES
 *********************/

#define TXD_PIN     (GPIO_NUM_17)
#define RXD_PIN     (GPIO_NUM_16)
#define STACK_SIZE  1024

/*********************
 *  EXTERN VARIABLE
 *********************/

extern TaskHandle_t connectWifi_task;
extern TaskHandle_t uart_rx_task;
extern TaskHandle_t uart_tx_task;

/*********************
 * ENTRY TASK FUNCTION
 *********************/

extern void startConnectWifiTask(void *arg);
extern void tx_task(void *arg);
extern void rx_task(void *arg);

/*********************
 *   INLINE FUNCTION
 *********************/

static inline void createMainTask(void)
{
    xTaskCreate(tx_task, 
                "uart_tx_task", 
                STACK_SIZE * 2, 
                NULL, 
                configMAX_PRIORITIES - 2, 
                &uart_tx_task);

    xTaskCreate(rx_task, 
                "uart_rx_task", 
                STACK_SIZE * 3, 
                NULL, 
                configMAX_PRIORITIES - 1, 
                &uart_rx_task);

    xTaskCreate(startConnectWifiTask, 
                "Wifi connect", 
                STACK_SIZE * 3, 
                NULL, 
                10, 
                &connectWifi_task);
}

#endif /* MAIN_H */