/*********************
 *      INCLUDES
 *********************/

#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"

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
 *  EXTERN VARIABLES
 **********************/

extern SemaphoreHandle_t mqtt_semaphore;

/**********************
 *     VARIABLES
 **********************/

char data_mqtt [10];

/**********************
 *  STATIC VARIABLES
 **********************/

static int8_t state_connect_mqtt = -1;
static uint8_t number_mqtt_subscribe = 0;

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
            number_mqtt_subscribe++;
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            sprintf(data_mqtt, "%.*s", event->data_len, event->data);
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

int8_t MQTT_app_state_connect(void)
{
    return state_connect_mqtt;
}