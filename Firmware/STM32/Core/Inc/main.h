/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_exti.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_pwr.h"
#include "stm32f1xx_ll_dma.h"
#include "stm32f1xx_ll_gpio.h"

  /* Private includes ----------------------------------------------------------*/
  /* USER CODE BEGIN Includes */

  /* USER CODE END Includes */

  /* Exported types ------------------------------------------------------------*/
  /* USER CODE BEGIN ET */

  /* USER CODE END ET */

  /* Exported constants --------------------------------------------------------*/
  /* USER CODE BEGIN EC */

  /* USER CODE END EC */

  /* Exported macro ------------------------------------------------------------*/
  /* USER CODE BEGIN EM */

  /* USER CODE END EM */

  /* Exported functions prototypes ---------------------------------------------*/
  void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define TFT_IRQ_Pin GPIO_PIN_1
#define TFT_IRQ_GPIO_Port GPIOA
#define TFT_DC_RS_Pin GPIO_PIN_2
#define TFT_DC_RS_GPIO_Port GPIOA
#define TFT_RESET_Pin GPIO_PIN_3
#define TFT_RESET_GPIO_Port GPIOA
#define SPI2_CS_Pin GPIO_PIN_12
#define SPI2_CS_GPIO_Port GPIOB

  /* USER CODE BEGIN Private defines */

  // UART TX EVENT
#define ON_WIFI_BIT                             (1 << 0)
#define OFF_WIFI_BIT                            (1 << 1)
#define CONNECT_WIFI_BIT                        (1 << 2)
#define CONNECT_MQTT_BIT                        (1 << 3)
#define MQTT_PUBLISH_BIT                        (1 << 5)

  // UART RX EVENT
#define NUMBER_WIFI_SCAN_BIT              (1 << 0) 
#define NAME_WIFI_SCAN_BIT                (1 << 1)
#define CONNECT_WIFI_SUCCESSFUL_BIT       (1 << 2)
#define CONNECT_WIFI_UNSUCCESSFUL_BIT     (1 << 3)
#define SSID_CONNECT_WIFI_SUCCESSFUL_BIT  (1 << 4)
#define REFUSE_CONNECT_MQTT_BIT           (1 << 5) // if haven't connect wifi
#define CONNECT_MQTT_SUCCESSFUL_BIT       (1 << 6)
#define CONNECT_MQTT_UNSUCCESSFUL_BIT     (1 << 7)
#define MQTT_SUBSCRIBE_BIT                (1 << 8)

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
} uart_tx_heading_t;

typedef enum
{
  HEADING_NUMBER_WIFI_SCAN = 0x01,
  HEADING_NAME_WIFI_SCAN,
  HEADING_CONNECT_WIFI_SUCCESSFUL,
  HEADING_CONNECT_WIFI_UNSUCCESSFUL,
  HEADING_SSID_CONNECT_WIFI_SUCCESS,
  HEADING_REFUSE_CONNECT_MQTT,
  HEADING_CONNECT_MQTT_SUCCESSFUL,
  HEADING_CONNECT_MQTT_UNSUCCESSFUL,
  HEADING_MQTT_SUBSCRIBE,
} uart_rx_heading_t;

typedef enum
{
    CONNECTED_WIFI,
    DISCONNECTED_WIFI,
    CONNECTED_MQTT,
    DISCONNECTED_MQTT,
} state_connect_network_t;

typedef struct
{
  uint8_t uart_buffer[10];
  uint8_t position_uart;
  uint8_t uart_flag;
} UART_t;
  /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
