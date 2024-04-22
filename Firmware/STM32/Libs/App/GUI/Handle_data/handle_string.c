#include <stdio.h>
#include <string.h>
#include "stdint.h"
#include "handle_string.h"

extern uint8_t password[8];
char ssid[30] ;
char ssid_pass[30];

// matching string
void data_merge(const char *ssid, const char *password, char *ssid_pass) 
{
    sprintf(ssid_pass, "%s\r%s\r", ssid, password);
}
