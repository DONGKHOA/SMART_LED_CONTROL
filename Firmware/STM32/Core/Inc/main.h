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
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

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
#define TFT_IRQ_EXTI_IRQn EXTI1_IRQn
#define TFT_DC_RS_Pin GPIO_PIN_2
#define TFT_DC_RS_GPIO_Port GPIOA
#define TFT_RESET_Pin GPIO_PIN_3
#define TFT_RESET_GPIO_Port GPIOA
#define SPI2_CS_Pin GPIO_PIN_12
#define SPI2_CS_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define RX_BUFFER_SIZE 					          100

#define TIME_REQUEST_SCAN                 5000
#define TIME_REFRESH_DISPLAY              100

// UART TX EVENT
#define ON_WIFI_BIT                       (1 << 0)
#define OFF_WIFI_BIT                      (1 << 1)
#define CONNECT_WIFI_BIT                  (1 << 2)
#define CONNECT_MQTT_BIT                  (1 << 3)
#define MQTT_PUBLISH_BIT                  (1 << 5)

// UART RX EVENT
#define NUMBER_WIFI_SCAN_BIT              (1 << 0) 
#define NAME_WIFI_SCAN_BIT                (1 << 1)
#define CONNECT_WIFI_SUCCESSFUL_BIT       (1 << 2)
#define CONNECT_WIFI_UNSUCCESSFUL_BIT     (1 << 3)
#define REFUSE_CONNECT_MQTT_BIT           (1 << 4) // if haven't connect wifi
#define CONNECT_MQTT_SUCCESSFUL_BIT       (1 << 5)
#define CONNECT_MQTT_UNSUCCESSFUL_BIT     (1 << 6)
#define MQTT_SUBSCRIBE_BIT                (1 << 7)
#define DETECT_TOUCH_SCREEN_BIT           (1 << 8)
#define REFRESH_DISPLAY_BIT               (1 << 9)

/**********************
 *      TYPEDEFS
 **********************/
typedef enum 
{
  HEADING_WIFI = 0x01,
  HEADING_CONNECT_WIFI,
  HEADING_CONNECT_MQTT,
  HEADING_MQTT_PUBLISH,
} uart_tx_heading_t;

typedef enum
{
  HEADING_RECEIVE_NUMBER_WIFI_SCAN = 0x01,
  HEADING_RECEIVE_NAME_WIFI_SCAN,
  HEADING_RECEIVE_CONNECT_WIFI,
  HEADING_RECEIVE_CONNECT_MQTT,
  HEADING_MQTT_SUBSCRIBE,
} uart_rx_heading_t;

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
