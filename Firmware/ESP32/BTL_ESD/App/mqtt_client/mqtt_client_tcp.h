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
 *   EXTERN VARIABLE
 **********************/

extern int8_t state_connect_mqtt;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void mqtt_app_get_data(MQTT_Client_Data_t *MQTT_Client);
void mqtt_app_start(MQTT_Client_Data_t *MQTT_Client, char *url);

#endif