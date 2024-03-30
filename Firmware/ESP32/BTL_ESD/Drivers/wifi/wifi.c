/*********************
 *      INCLUDES
 *********************/

#include "wifi.h"
#include "nvs_rw.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

/*********************
 *      DEFINES
 *********************/

#define WIFI_CONNECTED_BIT  BIT0
#define WIFI_FAIL_BIT       BIT1
#define MAXIMUM_RETRY       10
#define TAG                 "WIFI CONNECT"

/**********************
 *  STATIC VARIABLES
 **********************/

static uint8_t s_retry_num = 0;

static uint16_t volatile id = 0;    //id of ssid & pass
static nvs_handle_t volatile ssid_nvs[LIMIT_STORE_WIFI];
static nvs_handle_t volatile pass_nvs[LIMIT_STORE_WIFI];

static EventGroupHandle_t s_wifi_event_group;

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } 
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) 
    {
        if (s_retry_num < MAXIMUM_RETRY) 
        {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        } 
        else 
        {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGE(TAG,"connect to the AP fail");
    } 
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) 
    {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * The function `WIFI_Sta_Init` initializes the WiFi station interface.
 */
void WIFI_Sta_Init(void)
{
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);
}

/**
 * The function `WIFI_Scan` scans for nearby WiFi access points and retrieves their SSID names.
 * 
 * @param ssid_name The function `WIFI_Scan` scans for available WiFi access points and retrieves their
 * SSID names. The SSID names are stored in the `ssid_name` parameter, which is a pointer to a uint8_t
 * array where the SSID names will be written.
 * 
 * @return The function `WIFI_Scan` returns the total number of access points (APs) scanned and found
 * during the Wi-Fi scanning process.
 */
uint8_t WIFI_Scan(uint8_t * ssid_name)
{
    wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&config);

    esp_wifi_set_mode(WIFI_MODE_STA);

    esp_wifi_start();
    uint16_t number = SCAN_LIST_SIZE;
    wifi_ap_record_t ap_info[SCAN_LIST_SIZE];
    uint16_t ap_count = 0;
    memset(ap_info, 0, sizeof(ap_info));

    esp_wifi_scan_start(NULL, true);
    esp_wifi_scan_get_ap_records(&number, ap_info);
    esp_wifi_scan_get_ap_num(&ap_count);

    ESP_LOGI(TAG, "Total APs scanned = %u", ap_count);

    uint16_t ssid_name_pos = 0;
    uint8_t buffer[32];

    for (uint8_t i = 0; (i < SCAN_LIST_SIZE) && (i < ap_count); i++) 
    {
        uint16_t temp_pos = 0;
        sprintf((char *)buffer, "%s\r", ap_info[i].ssid);

        while (buffer[temp_pos] != '\r')
        {
            *(ssid_name + ssid_name_pos) = buffer[temp_pos];
            ssid_name_pos++;
            temp_pos++;
        }
        
        *(ssid_name + ssid_name_pos) = '\r';

        ESP_LOGI(TAG, "SSID \t\t%s", ap_info[i].ssid);
        ESP_LOGI(TAG, "Channel \t\t%d\n", ap_info[i].primary);
    }

    *(ssid_name + ssid_name_pos) = '\0';

    return ap_count;
}

/**
 * The function `WIFI_Connect` attempts to connect to a WiFi network using the provided SSID and
 * password, handling authentication modes and event notifications.
 * 
 * @param ssid The `ssid` parameter in the `WIFI_Connect` function is a pointer to an array of
 * characters representing the SSID (Service Set Identifier) of the Wi-Fi network you want to connect
 * to. The SSID is essentially the name of the Wi-Fi network.
 * @param password The code you provided is a function `WIFI_Connect` that attempts to connect to a
 * WiFi network using the provided SSID and password. The function initializes the WiFi configuration,
 * sets up event handlers, and then tries to connect to the network.
 * 
 * @return The function `WIFI_Connect` returns a value of type `WIFI_Status_t`, which is an enumeration
 * type. The possible return values are:
 * - `CONNECT_OK` if the connection to the Wi-Fi network was successful.
 * - `CONNECT_FAIL` if the connection to the Wi-Fi network failed.
 * - `UNEXPECTED_EVENT` if an unexpected event occurred during the connection process.
 */
WIFI_Status_t WIFI_Connect(uint8_t *ssid, uint8_t *password)
{
    esp_wifi_stop();
 
    s_wifi_event_group = xEventGroupCreate();

    wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&config);


    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    esp_event_handler_instance_register(WIFI_EVENT,
                                        ESP_EVENT_ANY_ID,
                                        &event_handler,
                                        NULL,
                                        &instance_any_id);
    esp_event_handler_instance_register(IP_EVENT,
                                        IP_EVENT_STA_GOT_IP,
                                        &event_handler,
                                        NULL,
                                        &instance_got_ip);

    wifi_config_t wifi_config =
    {
        .sta = 
        {
            /* Authmode threshold resets to WPA2 as default if password matches WPA2 standards (pasword len => 8).
             * If you want to connect the device to deprecated WEP/WPA networks, Please set the threshold value
             * to WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK and set the password with length and format matching to
             * WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK standards.
             */
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
        },
    };
    
    for (uint8_t i = 0; i < 32; i++)
    {
        wifi_config.sta.ssid[i] = *(ssid + i);
        wifi_config.sta.password[i] = *(password + i);
    }
    
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();

    ESP_LOGI(TAG, "Wifi_init_station finished.");

    /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
     * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
            WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY);

    /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
     * happened. */
    if (bits & WIFI_CONNECTED_BIT) 
    {
        s_retry_num = 0;
        ESP_LOGI(TAG, "Connected to ap SSID:%s password:%s",
                wifi_config.sta.ssid, wifi_config.sta.password);
        return CONNECT_OK;
    } 
    else if (bits & WIFI_FAIL_BIT) 
    {
        s_retry_num = 0;
        ESP_LOGE(TAG, "Failed to connect to SSID:%s, password:%s",
                wifi_config.sta.ssid, wifi_config.sta.password);
        return CONNECT_FAIL;
    } 
    else 
    {
        s_retry_num = 0;
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
        return UNEXPECTED_EVENT;
    }
}

uint16_t WIFI_Scan_NVS(void)
{
    id = 0;
    char key[32];
    // sprintf(key, "%d ssid", id);
    // while (nvs_get_str(ssid_nvs[id], key, ))
    // {
        
    // }
    return 0;
}

void WIFI_Store_NVS(uint8_t * ssid, uint8_t *password)
{

}