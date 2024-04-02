# FIRMWARE

## Khung truyền


HEADING | DATA | STOP |
--------|------|------|

**Trong đó các phần được truyền như sau:**

> HEADING: RX (ESP32) là TX (STM32) và ngược lại.
> 
> DATA: Có thể có hoặc không tùy vào loại HEADING
> 
> STOP: '\n'

**HEADING bên ESP32:**

```c
typedef enum 
{
    HEADING_ON_WIFI = 0x01,
    HEADING_OFF_WIFI,
    HEADING_CONNECT_WIFI,
    HEADING_CONNECT_MQTT,
    HEADING_AUTO_LED,
} uart_rx_heading_t;

typedef enum
{
    HEADING_SEND_NUMBER_WIFI_SCAN = 0x01,
    HEADING_SEND_NAME_WIFI_SCAN,
    HEADING_SEND_CONNECT_WIFI_SUCCESSFUL,
    HEADING_SEND_CONNECT_WIFI_UNSUCCESSFUL,
    HEADING_REFUSE_CONNECT_MQTT,
    HEADING_SEND_CONNECT_MQTT_SUCCESSFUL,
    HEADING_SEND_CONNECT_MQTT_UNSUCCESSFUL,
    HEADING_MQTT_PUBLISH,
    HEADING_MQTT_SUBSCRIBE,
} uart_tx_heading_t;
```
