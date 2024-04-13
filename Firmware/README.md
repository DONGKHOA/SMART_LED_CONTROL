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
    HEADING_MQTT_PUBLISH,
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
    HEADING_MQTT_SUBSCRIBE,
} uart_tx_heading_t;
```
**DATA**

-   MQTT:
    -   Subscribe: là dữ liệu được esp32 lấy về từ node red. Dữ liệu sẽ được cập sau mỗi 2s (`HEADING_MQTT_SUBSCRIBE`).
    -   Publish: là dữ liệu được esp32 đẩy lên node red. Dữ liệu sẽ được cập nhật trên node red khi có dữ liệu từ stm32 đưa qua cho esp32 (`HEADING_MQTT_PUBLISH`).
-   WIFI SCAN: 
    -   B1: esp32 sẽ gửi qua cho stm32 số lượng wifi đã scan ra được (`HEADING_SEND_NUMBER_WIFI_SCAN`).
    -   B2: esp32 sẽ gửi các tên wifi, mỗi tên wifi được cách nhau bởi `\r`.
-   WIFI Connect:
    -   esp32 có thể tự connect với các wifi đã được connect từ trước.
    -   nếu không tự connect thì esp32 sẽ đợi stm32 truyền ssid và pass: sẽ được truyền thông qua format (`HEADING_CONNECT_WIFI`, **ssid** `\r` **pass**, `\n`). với `\n` là **STOP**.
    -   Sau đó esp32 sẽ phản hồi lại connect thành công hay không thành công thông qua `HEADING_SEND_CONNECT_WIFI_SUCCESSFUL` hoặc `HEADING_SEND_CONNECT_WIFI_UNSUCCESSFUL`.
-   Mqtt connect:
    -   Người dùng sẽ nhập id của máy đang chạy node red.
    -   Nếu như chưa kết nối wifi thì sẽ bị refuse (`HEADING_REFUSE_CONNECT_MQTT`).
    -   Nếu như đã kết nối wifi thì esp32 sẽ phản hồi kết nối thành công hay không (`HEADING_SEND_CONNECT_MQTT_SUCCESSFUL` hoặc `HEADING_SEND_CONNECT_MQTT_UNSUCCESSFUL`).

**STOP**
Tất cả các khung truyền đều kết thúc bằng ký tự `\n`.