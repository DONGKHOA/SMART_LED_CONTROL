/*********************
 *      INCLUDES
 *********************/

#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"

#include "mqtt_client_tcp.h"

/*********************
 *      DEFINES
 *********************/

#define TAG  "MQTT"

/**********************
 *  STATIC VARIABLES
 **********************/

static char data [10];
static int8_t state_connect_mqtt = -1;

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%" PRIi32 "", base, event_id);

    esp_mqtt_event_handle_t event = event_data;

    switch ((esp_mqtt_event_id_t)event_id) 
    {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            state_connect_mqtt = 1;
            break;
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\n", event->topic_len, event->topic);
            sprintf(data ,"%.*s\n", event->data_len, event->data);
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGE(TAG, "MQTT_EVENT_DISCONNECTED");
            state_connect_mqtt = -1;
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
}

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * The function `MQTT_app_start` initializes an MQTT client with the provided URL and starts the
 * client.
 * 
 * @param MQTT_Client The `MQTT_Client` parameter is a pointer to a structure of type
 * `MQTT_Client_Data_t`.
 * @param url The `url` parameter in the `MQTT_app_start` function is a pointer to a character array
 * that represents the URI address of the MQTT broker to which the client will connect.
 */
void MQTT_app_start(MQTT_Client_Data_t *MQTT_Client, char *url)
{
    
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = url,
    };
    
    MQTT_Client->client = esp_mqtt_client_init(&mqtt_cfg);
    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event(MQTT_Client->client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(MQTT_Client->client);
}

/**
 * The function MQTT_app_get_data copies data from an array to a structure member until a null
 * character is encountered, and returns -1 if no data is copied or 1 otherwise.
 * 
 * @param MQTT_Client The function `MQTT_app_get_data` takes a pointer to a structure of type
 * `MQTT_Client_Data_t` as a parameter. This structure likely contains a data array that the function
 * manipulates. The function clears the data array, copies data from another array `data` into the `MQ
 * 
 * @return The function `MQTT_app_get_data` returns an `int8_t` value. If `i` is equal to 0, it returns
 * -1. Otherwise, it returns 1.
 */
int8_t MQTT_app_get_data(MQTT_Client_Data_t *MQTT_Client)
{
    memset(MQTT_Client->data, '\0', sizeof(MQTT_Client->data));
    uint8_t i;
    for (i = 0; i < sizeof(MQTT_Client->data); i++)
    {
        MQTT_Client->data[i] = data[i];
        if (data[i] == '\n')
        {
            MQTT_Client->data[i] = '\0';
            memset(data, '\0', sizeof(data));
            break;
        }
        
    }

    if(i == 0)  return -1;
    else        return 1;
}

int8_t MQTT_app_state_connect(void)
{
    return state_connect_mqtt;
}