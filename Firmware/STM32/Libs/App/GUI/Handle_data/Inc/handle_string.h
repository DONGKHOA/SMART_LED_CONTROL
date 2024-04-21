/*
 * merge_wifi.h
 *
 *  Created on: Apr 20, 2024
 *      Author: ACER
 */

#ifndef APP_GUI_MERGE_DATA_MERGE_WIFI_H_
#define APP_GUI_MERGE_DATA_MERGE_WIFI_H_

void data_merge(const char *ssid, const char *password, char *ssid_pass); // merge string ssid\rpassword\r
void uartSendData(volatile uint8_t* data);

#endif /* APP_GUI_MERGE_DATA_MERGE_WIFI_H_ */
