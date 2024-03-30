#ifndef NVS_H_
#define NVS_H_

#include <stdint.h>
#include "esp_err.h"

esp_err_t NVS_Init(void);
esp_err_t NVS_WriteString(const char* name, const char* key, 
                            const char* stringVal);

esp_err_t NVS_Read_String(const char* name, const char* key, char* savedData);

#endif /* NVS_H_ */