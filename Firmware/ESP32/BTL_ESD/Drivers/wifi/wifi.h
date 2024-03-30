#ifndef WIFI_H_
#define WIFI_H_

/*********************
 *      INCLUDES
 *********************/

#include <stdint.h>

/*********************
 *      DEFINES
 *********************/

#define SCAN_LIST_SIZE      20
#define LIMIT_STORE_WIFI    10

/**********************
 *      TYPEDEFS
 **********************/

typedef enum
{
    CONNECT_OK = 1,
    CONNECT_FAIL,
    UNEXPECTED_EVENT,
}WIFI_Status_t;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void WIFI_Sta_Init(void);
uint8_t WIFI_Scan(uint8_t * ssid_name);
WIFI_Status_t WIFI_Connect(uint8_t *ssid, uint8_t *password);
uint16_t WIFI_Scan_NVS(void);
void WIFI_Store_NVS(uint8_t * ssid, uint8_t *password);

#endif