#ifndef WIFI_H_
#define WIFI_H_

/*********************
 *      INCLUDES
 *********************/

#include <stdint.h>

/*********************
 *      DEFINES
 *********************/

#define SCAN_LIST_SIZE      50
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
 *   EXTERN VARIABLE
 **********************/

extern int8_t state_connected_wifi;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void WIFI_StaInit(void);
uint8_t WIFI_Scan(uint8_t * data_name);
WIFI_Status_t WIFI_Connect(uint8_t *ssid, uint8_t *password);
int8_t WIFI_ScanNVS(uint8_t * ssid, uint8_t * pass);
void WIFI_StoreNVS(uint8_t * ssid, uint8_t *password);

#endif