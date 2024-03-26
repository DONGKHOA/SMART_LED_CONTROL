#ifndef WIFI_H_
#define WIFI_H_

#include <stdint.h>

#define SCAN_LIST_SIZE 20

typedef enum
{
    CONNECT_OK = 1,
    CONNECT_FAIL,
    UNEXPECTED_EVENT,
}WIFI_Status_t;

extern uint8_t ssid_name[32 * SCAN_LIST_SIZE];

void WIFI_Sta_Init(void);
uint8_t WIFI_Scan(void);
WIFI_Status_t WIFI_Connect(uint8_t *ssid, uint8_t *password);

#endif