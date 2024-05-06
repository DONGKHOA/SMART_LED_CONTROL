/*********************
 *      INCLUDES
 *********************/

#include "main.h"

/**********************
 *     VARIABLES
 **********************/

char buffer_uart_rx[RX_BUF_SIZE + 1];
char buffer_uart_tx[RX_BUF_SIZE + 1];
static char buffer_ssid_scan[200 + 1];

SemaphoreHandle_t mqtt_semaphore;

/**********************
 *  STATIC VARIABLES
 **********************/

static uint8_t volatile ssid[32];
static uint8_t volatile pass[32];
static uint8_t flag_connect_success = 0;

static char url_mqtt[30] = "mqtt://172.16.0.181:1883";
static MQTT_Client_Data_t mqtt_client_0;
static uint8_t is_publish = 0;
char ssid1[32];
char ssid2[32];
char ssid3[32];
char ssid4[32];
char ssid5[32];

// Task Handle

static TaskHandle_t uart_rx_task;
static TaskHandle_t uart_tx_task;
static TaskHandle_t wifiScan_task;
static TaskHandle_t wifiConnect_task;
static TaskHandle_t mqttConnect_task;
static TaskHandle_t mqttControlData_task;

// EventGroup Handle

static EventGroupHandle_t event_uart_rx_heading;
static EventGroupHandle_t event_uart_tx_heading;

// Timer Handle

static TimerHandle_t mqtt_subscribe_timer;

// Queue Handle

static QueueHandle_t mqtt_queue;

/******************************
 *  STATIC PROTOTYPE FUNCTION
 ******************************/

static void MQTT_timer_cb(TimerHandle_t xTimer);
static uint8_t getNumPage(uint8_t quantitySSID);

static void startUartRxTask(void *arg);
static void startUartTxTask(void *arg);
static void startWifiScan(void *arg);
static void startWifiConnectTask(void *arg);
static void startMQTTConnectTask(void *arg);
static void startMQTTControlDataTask(void *arg);

/********************
 *  MAIN FUNCTION
 ********************/

void app_main(void)
{
    printf("Main task start\n");

    printf("init module in main\n");
    NVS_Init();
    WIFI_StaInit();
    uartDriverInit(UART_NUM_1, TXD_PIN, RXD_PIN,
                   4800, UART_DATA_8_BITS,
                   UART_PARITY_DISABLE, UART_HW_FLOWCTRL_DISABLE,
                   UART_STOP_BITS_1);

    printf("init service in main\n");

    event_uart_rx_heading = xEventGroupCreate();
    event_uart_tx_heading = xEventGroupCreate();

    mqtt_subscribe_timer = xTimerCreate("time mqtt subscribe",
                                        TIME_MQTT_SUBSCRIBE / portTICK_PERIOD_MS,
                                        pdTRUE, (void *)0, MQTT_timer_cb);

    mqtt_queue = xQueueCreate(2, sizeof(uint8_t));

    mqtt_semaphore = xSemaphoreCreateCounting(2,0);

    printf("Task start initialization\n");

    xTaskCreate(startUartRxTask,
                "uart_rx_task",
                MIN_STACK_SIZE * 5,
                NULL,
                2,
                &uart_rx_task);

    xTaskCreate(startUartTxTask,
                "uart_tx_task",
                MIN_STACK_SIZE * 5,
                NULL,
                2,
                &uart_tx_task);

    xTaskCreate(startWifiScan,
                "Wifi scan",
                MIN_STACK_SIZE * 40,
                NULL,
                3,
                &wifiScan_task);

    xTaskCreate(startWifiConnectTask,
                "Wifi connect",
                MIN_STACK_SIZE * 3,
                NULL,
                4,
                &wifiConnect_task);

    xTaskCreate(startMQTTConnectTask,
                "Mqtt connect",
                MIN_STACK_SIZE * 3,
                NULL,
                5,
                &mqttConnect_task);

    xTaskCreate(startMQTTControlDataTask,
                "Mqtt control",
                MIN_STACK_SIZE * 3,
                NULL,
                6,
                &mqttControlData_task);

    printf("Task done initialization\n");
}

/********************
 *  STATIC FUNCTION
 ********************/

static void MQTT_timer_cb(TimerHandle_t xTimer)
{
    uint8_t buffer = MQTT_SUBSCRIBE;

    BaseType_t	xHigherPriorityTaskWoken = pdFALSE;
    xQueueSendFromISR(mqtt_queue, &buffer, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    xQueueSend(mqtt_queue, &buffer, 0);
}

static uint8_t getNumPage(uint8_t quantitySSID)
{
	uint8_t quantity = 0;
	if (quantitySSID == 0)
	{
		return 0;
	}
	else
	{
		quantity = quantitySSID / SSID_IN_PAGE;
		if ((quantitySSID % SSID_IN_PAGE) > 0)
			quantity++;
		return quantity;
	}
}

static uint8_t processingSSID(char *src, uint8_t numSSID, uint8_t page)
{
	char buffer[200 + 1];
    memcpy(buffer, src, strlen((char *) src));
    uint8_t arg_position = 0;
    
	// cut string
	char *temp_token = strtok(buffer, "\r");
	while (temp_token != NULL)
	{
        if (arg_position == (4 + (page - 1) * 5))
        {
            memcpy(ssid5, temp_token, strlen((char *)temp_token) + 1);
        }

        if (arg_position == (3 + (page - 1) * 5))
        {
            memcpy(ssid4, temp_token, strlen((char *)temp_token) + 1);
        }

        if (arg_position == (2 + (page - 1) * 5))
        {
            memcpy(ssid3, temp_token, strlen((char *)temp_token) + 1);
        }

        if (arg_position == (1 + (page - 1) * 5))
        {
            memcpy(ssid2, temp_token, strlen((char *)temp_token) + 1);
        }

        if (arg_position == (0 + (page - 1) * 5))
        {
            memcpy(ssid1, temp_token, strlen((char *)temp_token) + 1);
        }
		arg_position++;
        
		temp_token = strtok(NULL, "\r");
	}

	// Store data in array and set bit filed
    uint8_t numSSIDofPage = (numSSID - (page - 1) * SSID_IN_PAGE) > 
                SSID_IN_PAGE ? SSID_IN_PAGE : (numSSID - (page - 1) * SSID_IN_PAGE);

    return numSSIDofPage;
}

static void startUartRxTask(void *arg)
{
    uint8_t buffer_temp[RX_BUF_SIZE + 1];
    uint16_t pos_buffer_uart_rx = 0;
    uint8_t enable_bit = 0;
    int8_t data_heading = -1;
    uint16_t i;

    memset((void *)buffer_uart_rx, '\0', sizeof(buffer_uart_rx));

    while (1)
    {
        // uartSendData(UART_NUM_1, "hello\n");
        uint16_t rxBytes = uart_read_bytes(UART_NUM_1, &buffer_temp,
                                           RX_BUF_SIZE, 10 / portTICK_PERIOD_MS);
        if (rxBytes > 0)
        {
            for (i = 0; i < rxBytes; i++)
            {
                buffer_uart_rx[pos_buffer_uart_rx] = buffer_temp[i];
                if (buffer_uart_rx[pos_buffer_uart_rx] == '\n')
                {
                    enable_bit = 1;
                    buffer_uart_rx[pos_buffer_uart_rx] = '\0';
                    pos_buffer_uart_rx = 0;
                    break;
                }
                pos_buffer_uart_rx++;
            }

            if (enable_bit == 0)
            {
                continue;
            }

            data_heading = buffer_uart_rx[0];
            uint8_t size_buffer = strlen(buffer_uart_rx);

            for (i = 0; i < size_buffer; i++)
            {
                buffer_uart_rx[i] = buffer_uart_rx[i + 1];
            }

            switch (data_heading)
            {
            case HEADING_WIFI:
                if (memcmp(buffer_uart_rx, "OFF", strlen(buffer_uart_rx) + 1) == 0)
                {
                    xEventGroupSetBits(event_uart_rx_heading, OFF_WIFI_BIT);
                }
                else if (memcmp(buffer_uart_rx, "ON", strlen(buffer_uart_rx) + 1) == 0)
                {
                    xEventGroupSetBits(event_uart_rx_heading, ON_WIFI_BIT);
                }
                
                break;
            case HEADING_TAB_PAGE:
                if (memcmp(buffer_uart_rx, "NEXT", strlen(buffer_uart_rx) + 1) == 0)
                {
                    xEventGroupSetBits(event_uart_rx_heading, NEXT_PAGE_BIT);
                }
                else if (memcmp(buffer_uart_rx, "BACK", strlen(buffer_uart_rx) + 1) == 0)
                {
                    xEventGroupSetBits(event_uart_rx_heading, BACK_PAGE_BIT);
                }
                break;

            case HEADING_CONNECT_WIFI:
                xEventGroupSetBits(event_uart_rx_heading, CONNECT_WIFI_RX_BIT);
                flag_connect_success = 0;
                break;

            case HEADING_CONNECT_MQTT:
                xEventGroupSetBits(event_uart_rx_heading, CONNECT_MQTT_BIT);
                break;

            case HEADING_MQTT_PUBLISH:
                uint8_t buffer = MQTT_PUBLISH;
                xQueueSend(mqtt_queue, &buffer, 0);
                break;

            default:
                enable_bit = 0;
                memset((void *)buffer_uart_rx, '\0', sizeof(buffer_uart_rx));
                break;

                memset((void *)buffer_uart_rx, '\0', sizeof(buffer_uart_rx));
            }

            enable_bit = 0;
        }
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

static void startUartTxTask(void *arg)
{
    while (1)
    {
        EventBits_t uxBits = xEventGroupWaitBits(event_uart_tx_heading,
                                                 SCAN_WIFI_BIT |
                                                     SEND_CONNECT_WIFI_SUCCESSFUL_BIT |
                                                     SEND_CONNECT_WIFI_UNSUCCESSFUL_BIT |
                                                     SEND_REFUSE_CONNECT_MQTT_BIT |
                                                     SEND_CONNECT_MQTT_SUCCESSFUL_BIT |
                                                     SEND_CONNECT_MQTT_UNSUCCESSFUL_BIT |
                                                     SEND_MQTT_SUBSCRIBE,
                                                 pdTRUE, pdFALSE,
                                                 portMAX_DELAY);

        if (uxBits & SCAN_WIFI_BIT)
        {
            transmissionFrameData(HEADING_SEND_SCAN_WIFI, buffer_uart_tx);
        }

        if (uxBits & SEND_CONNECT_WIFI_SUCCESSFUL_BIT)
        {
            transmissionFrameData(HEADING_SEND_CONNECT_WIFI, buffer_uart_tx);
        }

        if (uxBits & SEND_REFUSE_CONNECT_MQTT_BIT)
        {
            transmissionFrameData(HEADING_SEND_CONNECT_MQTT, buffer_uart_tx);
        }

        if (uxBits & SEND_CONNECT_MQTT_SUCCESSFUL_BIT)
        {
            transmissionFrameData(HEADING_SEND_CONNECT_MQTT, buffer_uart_tx);
        }

        if (uxBits & SEND_CONNECT_MQTT_UNSUCCESSFUL_BIT)
        {
            transmissionFrameData(HEADING_SEND_CONNECT_MQTT, buffer_uart_tx);
        }

        if (uxBits & SEND_MQTT_SUBSCRIBE)
        {
            transmissionFrameData(HEADING_SEND_CONNECT_MQTT, buffer_uart_tx);
        }

        memset((void *)buffer_uart_tx, '\0', sizeof(buffer_uart_tx));
    }
}

static void startWifiScan(void *arg)
{
    uint8_t num_wifi_scan = 0;
    uint8_t limitNumPage = 1; 
    uint8_t numSSIDofPage;
    uint8_t page = 0;

    while (1)
    {
        EventBits_t uxBits = xEventGroupWaitBits(event_uart_rx_heading,
                                                 ON_WIFI_BIT |
                                                 NEXT_PAGE_BIT |
                                                 BACK_PAGE_BIT |
                                                     OFF_WIFI_BIT,
                                                 pdTRUE, pdFALSE,
                                                 portMAX_DELAY);

        if (uxBits & ON_WIFI_BIT)
        {
            /*
                - Store all ssid and pass scan to array buffer_uart_tx.
                - Check ssid & pass
                    - Matching ssid & pass in nvs ->
                    set bit CONNECT_WIFI_SCAN_BIT.
                    - Don't matching ssid & pass in nvs ->
                    store ssid & pass in nvs.
            */
            page = 1;
            num_wifi_scan = WIFI_Scan(buffer_ssid_scan);
            printf("%d\n", num_wifi_scan);

            // if (WIFI_state_connect() == CONNECT_OK)
            // {
            //     xEventGroupSetBits(event_uart_tx_heading,
            //                        SEND_CONNECT_WIFI_UNSUCCESSFUL_BIT);
            //     continue;
            // }

            if (matchingWIFIScan(buffer_ssid_scan, (uint8_t *)ssid, (uint8_t *)pass) != -1)
            {
                xEventGroupSetBits(event_uart_rx_heading,
                                   CONNECT_WIFI_SCAN_BIT);
            }

            limitNumPage = getNumPage(num_wifi_scan);
            numSSIDofPage = processingSSID(buffer_ssid_scan, num_wifi_scan, page);
            sprintf(buffer_uart_tx,"%s\r%d\r%s\r%s\r%s\r%s\r%s\r", "START", numSSIDofPage, ssid1, 
                                ssid2, ssid3, ssid4, ssid5);
            
            memset((void *)ssid1, '\0', sizeof(ssid1));
            memset((void *)ssid2, '\0', sizeof(ssid2));
            memset((void *)ssid3, '\0', sizeof(ssid3));
            memset((void *)ssid4, '\0', sizeof(ssid4));
            memset((void *)ssid5, '\0', sizeof(ssid5));

            xEventGroupSetBits(event_uart_tx_heading,
                               SCAN_WIFI_BIT);

        }

        if (uxBits & NEXT_PAGE_BIT)
        {
            char state_page[10];
            if(page < limitNumPage) page++;

            if (page == 1) strcpy(state_page, "START");
            else if (page == limitNumPage) strcpy(state_page, "END");
            else strcpy(state_page, "MID");

            numSSIDofPage = processingSSID(buffer_ssid_scan, num_wifi_scan, page);
            sprintf(buffer_uart_tx,"%s\r%d\r%s\r%s\r%s\r%s\r%s\r", state_page, numSSIDofPage,ssid1, 
                                ssid2, ssid3, ssid4, ssid5);
            
            memset((void *)ssid1, '\0', sizeof(ssid1));
            memset((void *)ssid2, '\0', sizeof(ssid2));
            memset((void *)ssid3, '\0', sizeof(ssid3));
            memset((void *)ssid4, '\0', sizeof(ssid4));
            memset((void *)ssid5, '\0', sizeof(ssid5));
            printf("%d\n", page);

            xEventGroupSetBits(event_uart_tx_heading,
                               SCAN_WIFI_BIT);
        }

        if (uxBits & BACK_PAGE_BIT)
        {
            char state_page[10];
            if(page > 1) page--;
            
            if (page == 1) strcpy(state_page, "START");
            else if (page == limitNumPage) strcpy(state_page, "END");
            else strcpy(state_page, "MID");

            numSSIDofPage = processingSSID(buffer_ssid_scan, num_wifi_scan, page);
            sprintf(buffer_uart_tx,"%s\r%d\r%s\r%s\r%s\r%s\r%s\r", state_page, numSSIDofPage,ssid1, 
                                ssid2, ssid3, ssid4, ssid5);

            memset((void *)ssid1, '\0', sizeof(ssid1));
            memset((void *)ssid2, '\0', sizeof(ssid2));
            memset((void *)ssid3, '\0', sizeof(ssid3));
            memset((void *)ssid4, '\0', sizeof(ssid4));
            memset((void *)ssid5, '\0', sizeof(ssid5));
            printf("%d\n", page);

            xEventGroupSetBits(event_uart_tx_heading,
                               SCAN_WIFI_BIT);
        }

        if (uxBits & OFF_WIFI_BIT)
        {
            esp_wifi_stop();
        }
    }
}

static void startWifiConnectTask(void *arg)
{

    while (1)
    {
        EventBits_t uxBits = xEventGroupWaitBits(event_uart_rx_heading,
                                                 CONNECT_WIFI_RX_BIT |
                                                     CONNECT_WIFI_SCAN_BIT,
                                                 pdTRUE, pdFALSE,
                                                 portMAX_DELAY);
        if (uxBits & CONNECT_WIFI_RX_BIT)
        {
            /*
                - Connect successful -> set bit SEND_CONNECT_WIFI_SUCCESSFUL
                                    -> store ssid & pass in nvs
                - Connect unsuccessful -> set bit SEND_CONNECT_WIFI_UNSUCCESSFUL
            */
            getSSID_PASS((uint8_t *)buffer_uart_rx, (uint8_t *)ssid, (uint8_t *)pass);

            if (WIFI_Connect((uint8_t *)ssid, (uint8_t *)pass) == CONNECT_OK)
            {
                if (flag_connect_success == 0)
                {
                    WIFI_StoreNVS((uint8_t *)ssid, (uint8_t *)pass);
                    flag_connect_success = 1;
                }

                strcpy(buffer_uart_tx, (const char *)ssid);
                xEventGroupSetBits(event_uart_tx_heading,
                                   SEND_CONNECT_WIFI_SUCCESSFUL_BIT);
            }
            else
            {
                strcpy(buffer_uart_tx, "FAILED");
                xEventGroupSetBits(event_uart_tx_heading,
                                   SEND_CONNECT_WIFI_UNSUCCESSFUL_BIT);
            }
        }

        if (uxBits & CONNECT_WIFI_SCAN_BIT)
        {
            if (WIFI_state_connect() == CONNECT_OK)
            {
                continue;
            }

            if (WIFI_Connect((uint8_t *)ssid, (uint8_t *)pass) == CONNECT_OK)
            {
                strcpy(buffer_uart_tx, (const char *)ssid);
                xEventGroupSetBits(event_uart_tx_heading,
                                   SEND_CONNECT_WIFI_SUCCESSFUL_BIT);
            }
        }
    }
}

static void startMQTTConnectTask(void *arg)
{
    while (1)
    {
        EventBits_t uxBits = xEventGroupWaitBits(event_uart_rx_heading,
                                                 CONNECT_MQTT_BIT,
                                                 pdTRUE, pdFALSE,
                                                 portMAX_DELAY);
        if (uxBits & CONNECT_MQTT_BIT)
        {
            /*
                If the system is connected to wifi, mqtt connection is allowed
                If the system is not connected to wifi, mqtt connection is not allowed
            */

            if (WIFI_state_connect() == CONNECT_OK)
            {
                sprintf(url_mqtt, "mqtt://%.*s:1883", strlen(buffer_uart_rx), buffer_uart_rx);
                MQTT_app_start(&mqtt_client_0, url_mqtt);
            }
            else
            {
                strcpy(buffer_uart_tx, "REFUSE");
                xEventGroupSetBits(event_uart_tx_heading,
                                   SEND_REFUSE_CONNECT_MQTT_BIT);
            }
        }
    }
}

static void startMQTTControlDataTask(void *arg)
{
    uint8_t buffer;

    while (1)
    {
        if (xQueueReceive(mqtt_queue, &buffer, portMAX_DELAY) == pdPASS)
        {
            if (WIFI_state_connect() != CONNECT_OK)
            {
                is_publish = 0;
                strcpy(buffer_uart_tx, "FAILED");
                xTimerStop(mqtt_subscribe_timer, 0);
                xEventGroupSetBits(event_uart_tx_heading,
                                   SEND_CONNECT_WIFI_UNSUCCESSFUL_BIT);
                continue;
            }

            if (MQTT_app_state_connect() != 1)
            {
                is_publish = 0;
                xTimerStop(mqtt_subscribe_timer, 0);
                strcpy(buffer_uart_tx, "FAILED");
                xEventGroupSetBits(event_uart_tx_heading,
                                   SEND_CONNECT_MQTT_UNSUCCESSFUL_BIT);
                continue;
            }

            if (buffer == MQTT_PUBLISH)
            {
                char state_led[4];
                char state_auto[4];
                char lux[5];
                char temperature[10];

                processingDataMQTTPublish(buffer_uart_rx, state_led, state_auto, lux, temperature);

                esp_mqtt_client_publish(mqtt_client_0.client, "state_led", (const char *)&state_led, 0, 1, 0);
                esp_mqtt_client_publish(mqtt_client_0.client, "state_auto_stm32", (const char *)&state_auto, 0, 1, 0);
                esp_mqtt_client_publish(mqtt_client_0.client, "lux", (const char *)&lux, 0, 1, 0);
                if (is_publish == 0)
                {
                    xTimerReset(mqtt_subscribe_timer, 0);
                    is_publish = 1;
                }
            }
            else if (buffer == MQTT_SUBSCRIBE)
            {
                char state_led[10];
                char state_auto_nodered[10];
                esp_mqtt_client_subscribe(mqtt_client_0.client, "button", 0);
                xSemaphoreTake(mqtt_semaphore, portMAX_DELAY);
                strcpy(state_led, data_mqtt);

                esp_mqtt_client_subscribe(mqtt_client_0.client, "state_auto_nodered", 0);
                xSemaphoreTake(mqtt_semaphore, portMAX_DELAY);
                strcpy(state_auto_nodered, data_mqtt);
                
                sprintf(buffer_uart_tx, "%s\r%s", state_led, state_auto_nodered);
                xEventGroupSetBits(event_uart_tx_heading,
                                   SEND_MQTT_SUBSCRIBE);
            }
        }
    }
}