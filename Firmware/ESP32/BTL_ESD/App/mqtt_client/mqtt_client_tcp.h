#ifndef MQTT_CLIENT_H_
#define MQTT_CLIENT_H_

/*********************
 *      INCLUDES
 *********************/

#include <stdint.h>
#include "mqtt_client.h"

/**********************
 *      TYPEDEFS
 **********************/

typedef struct 
{
    char data [10];
    esp_mqtt_client_handle_t client;
}MQTT_Client_Data_t;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void MQTT_app_start(MQTT_Client_Data_t *MQTT_Client, char *url);
int8_t MQTT_app_get_data(MQTT_Client_Data_t *MQTT_Client);
int8_t MQTT_app_get_event(void);

#endif